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
	maxBounceCount = 4;
}

Scene::Scene(std::shared_ptr<Primitive> *primitives, unsigned int numPrimitives) {
	simpleWorld = new SimpleWorld(primitives, numPrimitives);
	ambientLight = nullptr;
	mainCamera = nullptr;
	maxBounceCount = 4;
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
	float tMin, float& tMax, int bounceCount) const {
	IntersectionResult intersectionResult;
	
	float originalTMax = tMax;
	Primitive* closestPrimitive = simpleWorld->Intersect(ray, tMin, tMax, intersectionResult);
	
	if (closestPrimitive != nullptr) {
		auto intersectionPos = ray.GetPositionAtParam(tMax);
		ShadingInfo shadingInfo(intersectionResult.genericMetadata1,
								intersectionResult.genericMetadata2,
								intersectionResult.genericMetadata3,
								&intersectionResult.compoundPrimitiveToIntersectedPrim,
								ray.GetDirection(),
								intersectionPos);
		Material const * primitiveMaterial = closestPrimitive->GetMaterial(shadingInfo);
		Vector3 normalVec = closestPrimitive->GetNormal(shadingInfo);
		shadingInfo.normalVector = normalVec;
		
		// ambient light if available
		if (ambientLight != nullptr) {
			auto lightRadiance = ambientLight->GetRadiance(shadingInfo, *this);
			Color lightRadColor4 = Color(lightRadiance[0], lightRadiance[1], lightRadiance[2], 0.0);
			newColor += primitiveMaterial->GetAmbientColor(shadingInfo)*lightRadColor4;
		}
		
		bool isAreaLight = IsPrimitiveAssociatedWithLight(closestPrimitive);
		
		if (isAreaLight) {
			newColor +=
				primitiveMaterial->GetColorForAreaLight(shadingInfo);
		}
		else {
			AddContributionsFromLights(shadingInfo, normalVec, primitiveMaterial,
									   newColor);
			
			// do we need to recurse?
			if (primitiveMaterial->DoesSurfaceReflect() && bounceCount <
				maxBounceCount) {
				float reflectivity = primitiveMaterial->GetReflectivity();
				Vector3 reflectiveVec = primitiveMaterial->ReflectVectorOffSurface(normalVec, -ray.GetDirection());
				Ray reflectedRay(intersectionPos, reflectiveVec);
				Color reflectedColor(0.0f, 0.0f, 0.0f, 0.0f);
				Intersect(reflectedRay, reflectedColor,
						  0.001f, originalTMax, bounceCount+1);
				newColor += reflectedColor*reflectivity;
			}
		}
	}

	return closestPrimitive != nullptr;
}

void Scene::AddContributionsFromLights(ShadingInfo &shadingInfo, Vector3 & normalVec,
									   Material const * primitiveMaterial,
									   Color& newColor) const {
	for (auto currentLight : lights) {
		Vector3 vectorToLight;
		auto isAreaLight = currentLight->IsAreaLight();
		
		float projectionTerm = 0.0f;
		float vectorMagn = 0.0f;

		if (isAreaLight) {
			// TODO: area lighting here messes with shading info, and it's not
			// clear what it modifies. that's bad
			currentLight->ComputeAndStoreAreaLightInformation(shadingInfo);
			vectorToLight = shadingInfo.wi;
			vectorMagn = shadingInfo.wiScaled.Norm();
			projectionTerm = vectorToLight * normalVec;
			shadingInfo.wi = vectorToLight;			}
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
			Ray shadowFeelerRay(shadingInfo.intersectionPosition+
								vectorToLight*SHADOW_FEELER_EPSILON, vectorToLight);
			// test shadow feeler if light supports it!
			if (currentLight->CastsShadows() &&
				ShadowFeelerIntersectsAnObject(shadowFeelerRay, 0.0f, vectorMagn)) {
				inShadow = true;
			}
			
			if (!inShadow) {
				auto lightRadiance = currentLight->GetRadiance(shadingInfo, *this);

				Color lightRadColor4 = Color(lightRadiance[0], lightRadiance[1],
					lightRadiance[2], 0.0);
				newColor += isAreaLight ?
					primitiveMaterial->GetColorForAreaLight(shadingInfo)*
					lightRadColor4*
					currentLight->GeometricTerm(shadingInfo)/
					currentLight->PDF(shadingInfo)*
					projectionTerm
					:
					primitiveMaterial->GetDirectColor(shadingInfo)*
					lightRadColor4*projectionTerm;
			}
		}
	}
}

bool Scene::IsPrimitiveAssociatedWithLight(Primitive* primitive) const {
	for (auto currentLight : lights) {
		if (currentLight->GetPrimitive() == primitive) {
			return true;
		}
	}
	return false;
}

bool Scene::ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
	float tMax) const {
	auto* primitiveHit = simpleWorld->ShadowFeelerIntersectsAnObject(ray, tMin, tMax);
	return primitiveHit != nullptr;
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
