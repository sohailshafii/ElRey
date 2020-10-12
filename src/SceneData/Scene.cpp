#include "Scene.h"
#include <cstring>
#include <stdexcept>
#include <sstream>
#include "Math/CommonMath.h"
#include "IntersectionResult.h"
#include "SceneData/DirectionalLight.h"
#include "SceneData/AmbientLight.h"
#include "Primitives/CompoundObject.h"

Scene::Scene() {
	ambientLight = nullptr;
	mainCamera = nullptr;
}

Scene::Scene(Primitive **primitives, unsigned int numPrimitives) {
	for (unsigned int i = 0; i < numPrimitives; i++) {
		this->primitives.push_back(primitives[i]);
	}
	ambientLight = nullptr;
	mainCamera = nullptr;
}

Scene::~Scene() {
	CleanUpPrimitives();
	CleanUpLights();
	if (mainCamera != nullptr) {
		delete mainCamera;
	}
	if (ambientLight != nullptr) {
		delete ambientLight;
	}
}

void Scene::CleanUpPrimitives() {
	for(auto primitive : primitives) {
		delete primitive;
	}
	primitives.clear();
}

void Scene::CleanUpLights() {
	for(auto light : lights) {
		delete light;
	}
	lights.clear();
}

void Scene::AddPrimitive(Primitive *newPrimitive) {
	if (newPrimitive == nullptr) {
		throw std::runtime_error("Trying to add invalid primitive!");
	}
	std::string const & primitiveName = newPrimitive->GetName();
	for(Primitive* prim : primitives)
	{
		if (prim->GetName() == primitiveName) {
			std::stringstream exceptionMsg;
			exceptionMsg << "Trying to add primitive with duplicate name: " <<
				primitiveName << ". Primitive should have unique names otherwise " <<
				"compound objects cannot distinguish between children for normal " <<
				"vector calculation.\n";
			throw exceptionMsg;
		}
	}

	primitives.push_back(newPrimitive);
}

void Scene::AddPrimitives(Primitive **newPrimitives, unsigned int numNewPrimitives) {
	if (newPrimitives == nullptr || numNewPrimitives == 0) {
		throw std::runtime_error("Trying to add invalid primitives!");
	}

	for (unsigned int i = 0; i < numNewPrimitives; i++) {
		this->primitives.push_back(newPrimitives[i]);
	}
}

void Scene::RemovePrimitive(Primitive* primitiveToRemove) {
	if (primitiveToRemove == nullptr) {
		throw std::runtime_error("Trying to remove invalid primitive!");
	}
	std::string const & primitiveName = primitiveToRemove->GetName();
	for (auto it = primitives.begin(); it != primitives.end(); ) {
		if ((*it)->GetName() == primitiveName) {
			primitives.erase(it);
			break;
		} else {
			++it;
		}
	}
}

void Scene::AddLight(Light* newLight) {
	if (newLight == nullptr) {
		throw std::runtime_error("Trying to add invalid light!");
	}
	
	if (newLight->IsAmbient()) {
		SetAmbientLight(newLight);
		return;
	}
	
	lights.push_back(newLight);
}

void Scene::AddLights(Light** newLights, unsigned int numNewLights) {
	if (newLights == nullptr || numNewLights == 0) {
		throw std::runtime_error("Trying to add invalid lights!");
	}
	
	for(unsigned int i = 0; i < numNewLights; i++) {
		this->lights.push_back(newLights[i]);
	}
}

Primitive* Scene::FindPrimitiveByName(const std::string& name) {
	Primitive* foundPrimitive = nullptr;

	for (auto currentPrimitive : primitives) {
		if (currentPrimitive->GetName() == name) {
			foundPrimitive = currentPrimitive;
		}
	}

	return foundPrimitive;
}

void Scene::SetAmbientLight(Light* newAmbientLight) {
	if (ambientLight != nullptr) {
		delete ambientLight;
	}
	ambientLight = newAmbientLight;
}

bool Scene::Intersect(const Ray &ray, Color &newColor,
	float tMin, float& tMax) const {
	Primitive* closestPrimitive = nullptr;
	IntersectionResult intersectionResult;
	
	for (auto currentPrimitive : primitives) {
		if (currentPrimitive->UsedForInstancing()) {
			continue;
		}
		auto hitPrimitive = currentPrimitive->Intersect(ray, tMin, tMax,
														intersectionResult);
		if (hitPrimitive) {
			closestPrimitive = currentPrimitive;
		}
	}
	
	if (closestPrimitive != nullptr) {
		Material const * primitiveMaterial = closestPrimitive->GetMaterial();
		intersectionResult.SetIncomingRay(ray);
		auto intersectionPos = ray.GetPositionAtParam(tMax);
		intersectionResult.SetIntersectionT(tMax);
		intersectionResult.SetIntersectionPosition(intersectionPos);
		ParamsForNormal paramsForNormal(ray.GetDirection().Normalized(),
										intersectionPos, intersectionResult.GetGenericMetadata1(),
										intersectionResult.GetGenericMetadata2(),
										intersectionResult.GetGenericMetadata3(),
										intersectionResult.GetPrimitiveName());
		Vector3 normalVec = closestPrimitive->GetNormal(paramsForNormal);
		intersectionResult.SetIntersectionNormal(normalVec);
		
		// ambient light if available
		if (ambientLight != nullptr) {
			auto lightRadiance = ambientLight->GetRadiance(intersectionResult, *this);
			Color lightRadColor4 = Color(lightRadiance[0], lightRadiance[1], lightRadiance[2], 0.0);
			newColor += primitiveMaterial->GetAmbientColor(intersectionResult)*lightRadColor4;
		}
		
		for (auto currentLight : lights) {
			Vector3 vectorToLight;
			auto isAreaLight = currentLight->IsAreaLight();
			const Primitive* primitiveToExclude = nullptr;
			
			float projectionTerm = 0.0f;
			float vectorMagn = 0.0f;

			if (isAreaLight) {
				primitiveToExclude = currentLight->GetPrimitive();
				currentLight->ComputeAndStoreAreaLightInformation(intersectionResult);
				vectorToLight = intersectionResult.GetVectorToLight();
				vectorMagn = intersectionResult.GetVectorToLightScaled().Norm();
				projectionTerm = vectorToLight * normalVec;

				// if primitive we struck is area light itself, no need to test light visibility
				if (primitiveToExclude == closestPrimitive)
				{
					newColor +=
						primitiveMaterial->GetColorForAreaLight(intersectionResult);
					continue;
				}
			}
			else {
				// infinite lights don't rely on normalization
				auto lightDistanceInfinite = currentLight->IsLightDistanceInfinite();
				vectorToLight = -currentLight->GetDirectionFromPositionScaled(
																			  intersectionResult);
				if (lightDistanceInfinite) {
					vectorMagn = std::numeric_limits<float>::max();
				}
				else {
					vectorMagn = vectorToLight.Norm();
					intersectionResult.SetLightVectorScaled(vectorToLight);
					vectorToLight /= vectorMagn;
				}

				projectionTerm = vectorToLight * normalVec;
				intersectionResult.SetLightVector(vectorToLight);
			}
			
			if (projectionTerm > 0.0f) {
				bool inShadow = false;
				Ray shadowFeelerRay(intersectionPos+vectorToLight *SHADOW_FEELER_EPSILON, vectorToLight);
				// test shadow feeler if light supports it!
				if (currentLight->CastsShadows() &&
					ShadowFeelerIntersectsAnObject(shadowFeelerRay, 0.0f, vectorMagn,
						primitiveToExclude)) {
					inShadow = true;
				}
				
				if (!inShadow) {
					auto lightRadiance = currentLight->GetRadiance(intersectionResult, *this);

					Color lightRadColor4 = Color(lightRadiance[0], lightRadiance[1],
						lightRadiance[2], 0.0);
					newColor += isAreaLight ?
						primitiveMaterial->GetColorForAreaLight(intersectionResult)*
						lightRadColor4*projectionTerm*
						currentLight->GeometricTerm(intersectionResult)/
						currentLight->PDF(paramsForNormal)
						:
						primitiveMaterial->GetDirectColor(intersectionResult)*
						lightRadColor4*projectionTerm;
				}
			}
		}
	}

	return closestPrimitive != nullptr;
}

bool Scene::ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
	float tMax, const Primitive* primitiveToExclude) const {
	Ray rayToCast = ray;
	Vector3 originalDir = ray.GetDirection();
	Point3 originalOrigin = ray.GetOrigin();
	for (auto currentPrimitive : primitives) {
		if (currentPrimitive == primitiveToExclude ||
			currentPrimitive->UsedForInstancing()) {
			continue;
		}
		
		if (currentPrimitive->IntersectShadow(ray, tMin, tMax))
		{
			return true;
		}
	}
	return false;
}

void Scene::TranslateAndRotate(const Vector3& translation, float rightRotationDegrees,
						float upRotationDegrees) {
	if (!allowNavigation) {
		return;
	}
	mainCamera->TranslateAndRotate(translation, rightRotationDegrees, upRotationDegrees);
}

void Scene::CastIntoScene(unsigned char* pixels, unsigned int bytesPerPixel, float frameTime, bool BGRMode) {
	mainCamera->CastIntoScene(pixels, bytesPerPixel, this, frameTime, BGRMode);
}
