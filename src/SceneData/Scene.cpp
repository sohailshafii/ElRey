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

bool Scene::WhittedRaytrace(const Ray &ray, Color &newColor,
	float tMin, float tMax, int bounceCount) const {
	IntersectionResult intersectionResult;
	
	float tMaxHit = tMax;
	Primitive* closestPrimitive = simpleWorld->Intersect(ray, tMin, tMaxHit, intersectionResult);
	
	if (closestPrimitive != nullptr) {
		auto intersectionPos = ray.GetPositionAtParam(tMaxHit);
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
				primitiveMaterial->GetDirectColor(shadingInfo);
		}
		else {
			AddContributionsFromLights(shadingInfo, normalVec, primitiveMaterial,
									   newColor);
			std::vector<Material::SecondaryVectorInfo> secondaryVectors;
			primitiveMaterial->GetSecondaryVectors(shadingInfo, normalVec, -ray.GetDirection(), secondaryVectors);
			size_t numSecondaryVectors = secondaryVectors.size();
			// do we need to recurse?
			if (numSecondaryVectors > 0 && bounceCount < maxBounceCount) {
				for (size_t i = 0; i < numSecondaryVectors; i++) {
					auto & secondaryVecInfo = secondaryVectors[i];
					float vecCoeff = secondaryVecInfo.vecCoeff;
					auto & secondaryVec = secondaryVecInfo.direction;
					
					Ray reflectedRay(intersectionPos, secondaryVec);
					Color reflectedColor(0.0f, 0.0f, 0.0f, 0.0f);
					WhittedRaytrace(reflectedRay, reflectedColor,
									0.001f, tMax, bounceCount+1);
					newColor += reflectedColor*vecCoeff;
				}
			}
		}
	}

	return closestPrimitive != nullptr;
}

bool Scene::PathRaytrace(const Ray &ray, Color &newColor,
				  float tMin, float tMax, int bounceCount) const {
	IntersectionResult intersectionResult;
	std::vector<Material::DirectionSample> directionSamples;
	
	float tMaxHit = tMax;
	Primitive* closestPrimitive = simpleWorld->Intersect(ray, tMin, tMaxHit, intersectionResult);
	
	if (closestPrimitive != nullptr) {
		auto intersectionPos = ray.GetPositionAtParam(tMaxHit);
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
			// get light color but don't cast additional rays; stop here
			newColor +=
				primitiveMaterial->GetDirectColor(shadingInfo);
		}
		else {
			// only add contributions of direct lighting for first bounce
			if (bounceCount == 0) {
				AddContributionsFromLights(shadingInfo, normalVec, primitiveMaterial,
										   newColor);
			}
			
			// done
			if (bounceCount == maxBounceCount) {
				return;
			}
			
			// do we need to recurse? get a list of all vectors and pdfs to consider for path tracing
			primitiveMaterial->SampleColorAndDirections(shadingInfo, directionSamples);
			size_t numSamples = directionSamples.size();
			// cast secondary rays, where applicable
			for (size_t i = 0; i < numSamples; i++) {
				auto& currDirectionSample = directionSamples[i];
				auto& currWi = currDirectionSample.wi;
				auto& currColor = currDirectionSample.color;
				// if emissive sample, don't cast
				if (currWi.IsZeroVector()) {
					newColor += currColor;
					continue;
				}
				
				auto currPdf = currDirectionSample.pdf;
				float projectionTerm = shadingInfo.normalVector*currWi;
				Color bounceColor(0.0f, 0.0f, 0.0f, 0.0f);
				PathRaytrace(Ray(intersectionPos, currWi), bounceColor,
							 SHADOW_FEELER_EPSILON, tMax, bounceCount+1);
				newColor += currColor * bounceColor * projectionTerm/currPdf;
			}
		}
	}

	return closestPrimitive != nullptr;
}

void Scene::AddContributionsFromLights(ShadingInfo const & shadingInfo, Vector3 & normalVec,
									   Material const * primitiveMaterial,
									   Color& newColor) const {
	ShadingInfo oldShadingInfo = shadingInfo;
	ShadingInfo currShadingInfo = shadingInfo;
	
	for (auto currentLight : lights) {
		Vector3 vectorToLight;
		auto isAreaLight = currentLight->IsAreaLight();
		
		float projectionTerm = 0.0f;
		float vectorMagn = 0.0f;

		if (isAreaLight) {
			currentLight->ModifyShadingInfoForAreaLight(currShadingInfo);
			vectorToLight = currShadingInfo.wi;
			vectorMagn = currShadingInfo.wiScaled.Norm();
			projectionTerm = vectorToLight * normalVec;
			currShadingInfo.wi = vectorToLight;
		}
		else {
			// infinite lights don't rely on normalization
			auto lightDistanceInfinite = currentLight->IsLightDistanceInfinite();
			vectorToLight = -currentLight->GetDirectionFromPositionScaled(
																		  currShadingInfo);
			if (lightDistanceInfinite) {
				vectorMagn = std::numeric_limits<float>::max();
			}
			else {
				vectorMagn = vectorToLight.Norm();
				currShadingInfo.wiScaled = vectorToLight;
				vectorToLight /= vectorMagn;
			}

			projectionTerm = vectorToLight * normalVec;
			currShadingInfo.wi = vectorToLight;
		}
		
		if (projectionTerm > 0.0f) {
			bool inShadow = false;
			Ray shadowFeelerRay(currShadingInfo.intersectionPosition, vectorToLight);
			// test shadow feeler if light supports it!
			if (currentLight->CastsShadows()) {
				// if it's area, light prevent hitting light source
				auto lightPrimitive = currentLight->GetPrimitive();
				if (lightPrimitive != nullptr) {
					lightPrimitive->SetIgnoreShadowTest(true);
				}
				bool shadowFeelerHitSomething = simpleWorld->ShadowFeelerIntersectsAnObject(shadowFeelerRay,
						SHADOW_FEELER_EPSILON, vectorMagn);
				if (lightPrimitive != nullptr) {
					lightPrimitive->SetIgnoreShadowTest(false);
				}
				if (shadowFeelerHitSomething) {
					continue;
				}
			}
			
			auto lightRadiance = currentLight->GetRadiance(currShadingInfo, *this);

			Color lightRadColor4 = Color(lightRadiance[0], lightRadiance[1],
				lightRadiance[2], 0.0);
			if (isAreaLight) {
				newColor += primitiveMaterial->GetDirectColor(currShadingInfo)*
					currentLight->GeometricTerm(currShadingInfo)/
					currentLight->PDF(currShadingInfo)*
					lightRadColor4*projectionTerm;
				// restore old shading info if this was an area light
				// that's because area lighting modifies shading record
				currShadingInfo = oldShadingInfo;
			}
			else {
				newColor += primitiveMaterial->GetDirectColor(currShadingInfo)*
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
