#include "Scene.h"
#include <cstring>
#include <stdexcept>
#include <sstream>
#include "Math/CommonMath.h"
#include "IntersectionResult.h"
#include "SceneData/DirectionalLight.h"
#include "SceneData/AmbientLight.h"
#include "Primitives/CompoundObject.h"
#include "AccelerationStructures/GridAccelerator.h"
#include "AccelerationStructures/SimpleWorld.h"

Scene::Scene(BaseAccelerator::AcceleratorType acceleratorType) {
	if (acceleratorType == BaseAccelerator::AcceleratorType::SimpleWorld) {
		baseAccelerator = new SimpleWorld();
	}
	else {
		baseAccelerator = new GridAccelerator();
	}
	
	ambientLight = nullptr;
	mainCamera = nullptr;
}

Scene::Scene(Primitive **primitives, unsigned int numPrimitives,
			 BaseAccelerator::AcceleratorType acceleratorType) {
	if (acceleratorType == BaseAccelerator::AcceleratorType::SimpleWorld) {
		baseAccelerator = new SimpleWorld(primitives, numPrimitives);
	}
	else {
		baseAccelerator = new GridAccelerator(primitives, numPrimitives);
	}
	
	ambientLight = nullptr;
	mainCamera = nullptr;
}

Scene::~Scene() {
	if (baseAccelerator != nullptr) {
		delete baseAccelerator;
	}
	CleanUpLights();
	if (mainCamera != nullptr) {
		delete mainCamera;
	}
	if (ambientLight != nullptr) {
		delete ambientLight;
	}
}

void Scene::CleanUpLights() {
	for(auto light : lights) {
		delete light;
	}
	lights.clear();
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

void Scene::SetAmbientLight(Light* newAmbientLight) {
	if (ambientLight != nullptr) {
		delete ambientLight;
	}
	ambientLight = newAmbientLight;
}

bool Scene::Intersect(const Ray &ray, Color &newColor,
	float tMin, float& tMax) const {
	IntersectionResult intersectionResult;
	
	Primitive* closestPrimitive = baseAccelerator->Intersect(ray, tMin, tMax, intersectionResult);
	
	if (closestPrimitive != nullptr) {
		Material const * primitiveMaterial = closestPrimitive->GetMaterial(intersectionResult);
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
				currentLight->ComputeAndStoreAreaLightInformation(intersectionResult, paramsForNormal);
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
				Ray shadowFeelerRay(intersectionPos+vectorToLight*SHADOW_FEELER_EPSILON, vectorToLight);
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
	auto* primitiveHit = baseAccelerator->ShadowFeelerIntersectsAnObject(ray, tMin, tMax);
	return (primitiveHit != nullptr && primitiveHit != primitiveToExclude);
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
