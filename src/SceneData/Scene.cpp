#include "Scene.h"
#include <cstring>
#include <stdexcept>
#include <sstream>
#include "Math/CommonMath.h"
#include "SceneData/ShadingInfo.h"
#include "SceneData/DirectionalLight.h"
#include "SceneData/AmbientLight.h"
#include "Primitives/CompoundObject.h"
#include "WorldData/SimpleWorld.h"

Scene::Scene() {
	simpleWorld = new SimpleWorld();
	
	ambientLight = nullptr;
	mainCamera = nullptr;
}

Scene::Scene(std::shared_ptr<Primitive> *primitives, unsigned int numPrimitives) {
	simpleWorld = new SimpleWorld(primitives, numPrimitives);
	ambientLight = nullptr;
	mainCamera = nullptr;
}

Scene::~Scene() {
	if (simpleWorld != nullptr) {
		delete simpleWorld;
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
	
	Primitive* closestPrimitive = simpleWorld->Intersect(ray, tMin, tMax, intersectionResult);
	
	if (closestPrimitive != nullptr) {
		ShadingInfo shadingInfo(intersectionResult.genericMetadata1,
								intersectionResult.genericMetadata2,
								intersectionResult.genericMetadata3,
								&intersectionResult.compoundPrimitiveToIntersectedPrim,
								ray.GetDirection(),
								ray.GetPositionAtParam(tMax));
		Material const * primitiveMaterial = closestPrimitive->GetMaterial(shadingInfo);
		Vector3 normalVec = closestPrimitive->GetNormal(shadingInfo);
		shadingInfo.normalVector = normalVec;
		
		// ambient light if available
		if (ambientLight != nullptr) {
			auto lightRadiance = ambientLight->GetRadiance(shadingInfo, *this);
			Color lightRadColor4 = Color(lightRadiance[0], lightRadiance[1], lightRadiance[2], 0.0);
			newColor += primitiveMaterial->GetAmbientColor(shadingInfo)*lightRadColor4;
		}
		
		for (auto currentLight : lights) {
			Vector3 vectorToLight;
			auto isAreaLight = currentLight->IsAreaLight();
			const Primitive* primitiveToExclude = nullptr;
			
			float projectionTerm = 0.0f;
			float vectorMagn = 0.0f;

			if (isAreaLight) {
				primitiveToExclude = currentLight->GetPrimitive();
				currentLight->ComputeAndStoreAreaLightInformation(shadingInfo);
				vectorToLight = shadingInfo.wi;
				vectorMagn = shadingInfo.wiScaled.Norm();
				projectionTerm = vectorToLight * normalVec;
				shadingInfo.wi = vectorToLight;

				// if primitive we struck is area light itself, no need to test light visibility
				if (primitiveToExclude == closestPrimitive)
				{
					newColor +=
						primitiveMaterial->GetColorForAreaLight(shadingInfo);
					continue;
				}
			}
			else {
				// infinite lights don't rely on normalization
				auto lightDistanceInfinite = currentLight->IsLightDistanceInfinite();
				vectorToLight = -currentLight->GetDirectionFromPositionScaled(
																			  shadingInfo);
				if (lightDistanceInfinite) {
					vectorMagn = std::numeric_limits<float>::max();
				}
				else {
					vectorMagn = vectorToLight.Norm();
					shadingInfo.wiScaled = vectorToLight;
					vectorToLight /= vectorMagn;
				}

				projectionTerm = vectorToLight * normalVec;
				shadingInfo.wi = vectorToLight;
			}
			
			if (projectionTerm > 0.0f) {
				bool inShadow = false;
				Ray shadowFeelerRay(shadingInfo.intersectionPosition+vectorToLight*SHADOW_FEELER_EPSILON, vectorToLight);
				// test shadow feeler if light supports it!
				if (currentLight->CastsShadows() &&
					ShadowFeelerIntersectsAnObject(shadowFeelerRay, 0.0f, vectorMagn,
						primitiveToExclude)) {
					inShadow = true;
				}
				
				if (!inShadow) {
					auto lightRadiance = currentLight->GetRadiance(shadingInfo, *this);

					Color lightRadColor4 = Color(lightRadiance[0], lightRadiance[1],
						lightRadiance[2], 0.0);
					newColor += isAreaLight ?
						primitiveMaterial->GetColorForAreaLight(shadingInfo)*
						lightRadColor4*projectionTerm*
						currentLight->GeometricTerm(shadingInfo)/
						currentLight->PDF(shadingInfo)
						:
						primitiveMaterial->GetDirectColor(shadingInfo)*
						lightRadColor4*projectionTerm;
				}
			}
		}
	}

	return closestPrimitive != nullptr;
}

bool Scene::ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
	float tMax, const Primitive* primitiveToExclude) const {
	auto* primitiveHit = simpleWorld->ShadowFeelerIntersectsAnObject(ray, tMin, tMax);
	// TODO: we probably don't need this, we wouldn't cast a shadow feeler
	// from light primitive to itself anyway
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
