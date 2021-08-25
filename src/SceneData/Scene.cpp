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
	
float Scene::WhittedRaytrace(const Ray &ray, Color3 &newColor,
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
								intersectionPos,
								intersectionResult.u,
								intersectionResult.v);
		shadingInfo.intersectionPositionLocal =  closestPrimitive->ComputeLocalIntersectionPoint(shadingInfo, intersectionPos);
		Material const * primitiveMaterial = closestPrimitive->GetMaterial(shadingInfo);
		Vector3 normalVec = closestPrimitive->GetNormal(shadingInfo);
		shadingInfo.normalVector = normalVec;
		
		// ambient light if available
		if (ambientLight != nullptr) {
			Color3 lightRadiance = ambientLight->GetRadiance(shadingInfo, *this);
			Color3 ambientColor = primitiveMaterial->GetAmbientColor(shadingInfo);
			newColor += ambientColor * lightRadiance;
		}
		
		bool isAreaLight = IsPrimitiveAssociatedWithLight(closestPrimitive);
		
		if (isAreaLight) {
			newColor += primitiveMaterial->GetDirectColor(shadingInfo);
		}
		else {
			Color3 colorFromLights = AddContributionsFromLights(shadingInfo, normalVec, primitiveMaterial);
			newColor += colorFromLights;
			std::vector<Material::SecondaryVectorInfo> secondaryVectors;
			primitiveMaterial->GetSecondaryVectors(shadingInfo, secondaryVectors);
			size_t numSecondaryVectors = secondaryVectors.size();
			// do we need to recurse?
			if (numSecondaryVectors > 0 && bounceCount < maxBounceCount) {
				for (size_t i = 0; i < numSecondaryVectors; i++) {
					auto & secondaryVecInfo = secondaryVectors[i];
					float vecCoeff = secondaryVecInfo.vecCoeff;
					auto & secondaryVec = secondaryVecInfo.direction;
					
					Ray secondaryRay(intersectionPos, secondaryVec);
					Color3 tracedColor = Color3::Black();
					float tracedLen = WhittedRaytrace(secondaryRay, tracedColor,
													  0.001f, tMax, bounceCount+1);
					Color3 returnedColor = secondaryVecInfo.colorComp*tracedColor*vecCoeff;
					// TODO: verify color filter
					if (secondaryVecInfo.useColorFilter && tracedLen > 0.0f) {
						returnedColor *= (secondaryVecInfo.colorFilter^tracedLen);
					}
					newColor += returnedColor;
				}
			}
		}
	}

	return closestPrimitive != nullptr ? tMaxHit : 0.0f;
}

float Scene::PathRaytrace(const Ray &ray, Color3 &newColor,
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
								intersectionPos,
								intersectionResult.u,
								intersectionResult.v);
		shadingInfo.intersectionPositionLocal =  closestPrimitive->ComputeLocalIntersectionPoint(shadingInfo, intersectionPos);
		Material const * primitiveMaterial = closestPrimitive->GetMaterial(shadingInfo);
		Vector3 normalVec = closestPrimitive->GetNormal(shadingInfo);
		shadingInfo.normalVector = normalVec;
		
		// ambient light if available
		if (ambientLight != nullptr) {
			Color3 lightRadiance = ambientLight->GetRadiance(shadingInfo, *this);
			Color3 ambientColor = primitiveMaterial->GetAmbientColor(shadingInfo);
			newColor += ambientColor * lightRadiance;
		}
		
		bool isAreaLight = IsPrimitiveAssociatedWithLight(closestPrimitive);
		
		if (isAreaLight) {
			// get light color but don't cast additional rays; stop here
			newColor += primitiveMaterial->GetDirectColor(shadingInfo);
		}
		else {
			// only add contributions of direct lighting for first bounce
			if (bounceCount == 0) {
				newColor += AddContributionsFromLights(shadingInfo, normalVec, primitiveMaterial);
			}
			
			// done
			if (bounceCount == maxBounceCount) {
				return 0.0f;
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
				Color3 bounceColor = Color3::Black();
				PathRaytrace(Ray(intersectionPos, currWi), bounceColor,
							 SHADOW_FEELER_EPSILON, tMax, bounceCount+1);
				newColor += currColor * bounceColor * projectionTerm/currPdf;
			}
		}
	}

	return closestPrimitive != nullptr ? tMaxHit : 0.0f;
}

Color3 Scene::AddContributionsFromLights(ShadingInfo const & shadingInfo,
										 Vector3 & normalVec,
										 Material const * primitiveMaterial) const {
	ShadingInfo oldShadingInfo = shadingInfo;
	ShadingInfo currShadingInfo = shadingInfo;
	Color3 newColor = Color3::Black();
	
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
			vectorToLight = -currentLight->GetDirectionFromPositionScaled(currShadingInfo);
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
			
			Color3 lightRadiance = currentLight->GetRadiance(currShadingInfo, *this);
			Color3 directColor = primitiveMaterial->GetDirectColor(currShadingInfo);
			if (isAreaLight) {
				newColor += directColor*
					currentLight->GeometricTerm(currShadingInfo)/
					currentLight->PDF(currShadingInfo)*
					lightRadiance*projectionTerm;
				// restore old shading info if this was an area light
				// that's because area lighting modifies shading record
				currShadingInfo = oldShadingInfo;
			}
			else {
				newColor += directColor*lightRadiance*projectionTerm;
			}
		}
	}
	
	return newColor;
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
