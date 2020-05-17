#include "Scene.h"
#include <stdexcept>
#include <cstring>
#include "Math/CommonMath.h"
#include "IntersectionResult.h"
#include "SceneData/DirectionalLight.h"
#include "SceneData/AmbientLight.h"

Scene::Scene() {
	primitives = nullptr;
	numPrimitives = 0;

	lights = nullptr;
	ambientLight = nullptr;
	numLights = 0;
	mainCamera = nullptr;
}

Scene::Scene(Primitive **primitives, unsigned int numPrimitives) {
	this->primitives = primitives;
	this->numPrimitives = numPrimitives;
	lights = nullptr;
	ambientLight = nullptr;
	mainCamera = nullptr;
}

Scene::~Scene() {
	CleanUpPrimitives(this->primitives, this->numPrimitives);
	CleanUpLights(this->lights, this->numLights);
	if (mainCamera != nullptr) {
		delete mainCamera;
	}
	if (ambientLight != nullptr) {
		delete ambientLight;
	}
}

void Scene::CleanUpPrimitives(Primitive **primitivesToClean, unsigned int
	numToClean) {
	if (primitivesToClean != nullptr) {
		for(unsigned int i = 0; i < numToClean; i++) {
			delete primitivesToClean[i];
		}
		delete [] primitivesToClean;
	}
}

void Scene::CleanUpLights(Light** lightsToClean, unsigned int numToClean) {
	if (lightsToClean != nullptr) {
		for (unsigned int i = 0; i < numToClean; i++) {
			delete lightsToClean[i];
		}
	}
	delete [] lightsToClean;
}

void Scene::AddPrimitive(Primitive *newPrimitive) {
	if (newPrimitive == nullptr) {
		throw std::runtime_error("Trying to add invalid primitive!");
	}

	if (primitives == nullptr) {
		this->numPrimitives = 1;
		this->primitives = new Primitive*[numPrimitives];
		this->primitives[0] = newPrimitive;
	}
	else {
		auto oldPrimitives = this->primitives;
		auto numOldPrimitives = this->numPrimitives;
		this->numPrimitives = 1 + numOldPrimitives;
		this->primitives = new Primitive*[this->numPrimitives];
		memcpy(this->primitives, oldPrimitives, numOldPrimitives*
			sizeof(Primitive*));
		this->primitives[this->numPrimitives-1] = newPrimitive;
		delete[] oldPrimitives;
	}
}

void Scene::AddPrimitives(Primitive **newPrimitives, unsigned int numNewPrimitives) {
	if (newPrimitives == nullptr || numNewPrimitives == 0) {
		throw std::runtime_error("Trying to add invalid primitives!");
	}

	if (this->primitives == nullptr) {
		this->primitives = newPrimitives;
		this->numPrimitives = numNewPrimitives;
	}
	else {
		auto oldPrimitives = this->primitives;
		auto numOldPrimitives = this->numPrimitives;
		this->numPrimitives = numNewPrimitives + numOldPrimitives;
		this->primitives = new Primitive*[this->numPrimitives];
		memcpy(this->primitives, oldPrimitives, numOldPrimitives*
			sizeof(Primitive*));
		memcpy(&this->primitives[numOldPrimitives], newPrimitives,
			numNewPrimitives*sizeof(Primitive*));
		delete[] oldPrimitives;
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

	if (lights == nullptr) {
		this->numLights = 1;
		this->lights = new Light * [numLights];
		this->lights[0] = newLight;
	}
	else {
		// TODO: vectorize
		auto oldLights = this->lights;
		auto numOldLights = this->numLights;
		this->numLights = 1 + numOldLights;
		this->lights = new Light * [this->numLights];
		memcpy(this->lights, oldLights, numOldLights *
			sizeof(Light*));
		this->lights[this->numLights - 1] = newLight;
		delete[] oldLights;
	}
}

void Scene::AddLights(Light** newLights, unsigned int numNewLights) {
	if (newLights == nullptr || numNewLights == 0) {
		throw std::runtime_error("Trying to add invalid lights!");
	}

	if (lights == nullptr) {
		this->lights = newLights;
		this->numLights = numNewLights;
	}
	else {
		// TODO: vectorize
		auto oldLights = this->lights;
		auto numOldLights = this->numLights;
		this->numLights = numNewLights + numOldLights;
		this->lights = new Light * [this->numLights];
		memcpy(this->lights, oldLights, numOldLights *
			sizeof(Light*));
		memcpy(&this->lights[numOldLights], newLights,
			numNewLights * sizeof(Light*));
		delete[] oldLights;
	}
}

Primitive* Scene::FindPrimitiveByName(const std::string& name) {
	Primitive* foundPrimitive = nullptr;

	for (unsigned int primIndex = 0; primIndex < numPrimitives;
		primIndex++) {
		Primitive* currentPrimitive = primitives[primIndex];
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
	for (unsigned int i = 0; i < numPrimitives; i++) {
		auto currentPrimitive = this->primitives[i];
		auto hitPrimitive = this->primitives[i]->Intersect(ray, tMin, tMax, intersectionResult);
		if (hitPrimitive) {
			closestPrimitive = currentPrimitive;
		}
	}
	
	if (closestPrimitive != nullptr) {
		std::shared_ptr<Material> primitiveMaterial = closestPrimitive->GetMaterial();
		intersectionResult.SetIncomingRay(ray);
		
		auto intersectionPos = ray.GetPositionAtParam(tMax);
		intersectionResult.SetIntersectionT(tMax);
		intersectionResult.SetIntersectionPosition(intersectionPos);
		// TODO: don't always need to calculate normal
		Vector3 normalVec = closestPrimitive->GetNormalAtPosition(intersectionPos);
		intersectionResult.SetIntersectionNormal(normalVec);
		
		// ambient light if available
		if (ambientLight != nullptr) {
			auto lightRadiance = ambientLight->GetRadiance(intersectionResult, *this);
			Color lightRadColor4 = Color(lightRadiance[0], lightRadiance[1], lightRadiance[2], 0.0);
			newColor += primitiveMaterial->GetAmbientColor(intersectionResult)*lightRadColor4;
		}
		
		for (unsigned int i = 0; i < numLights; i++) {
			auto currentLight = this->lights[i];
			Vector3 vectorToLight;
			auto isAreaLight = currentLight->IsAreaLight();
			const Primitive* primitiveToExclude = nullptr;
			
			if (isAreaLight) {
				currentLight->ComputeAndStoreAreaLightInformation(intersectionResult);
				vectorToLight = intersectionResult.GetVectorToLight();
				primitiveToExclude = currentLight->GetPrimitive();
			}
			else {
				vectorToLight = -currentLight->GetDirectionFromPosition(
					intersectionResult);
			}
			
			float vectorMagn = vectorToLight.Norm();
			vectorToLight /= vectorMagn;
			float projectionTerm = vectorToLight*normalVec;
			if (projectionTerm > 0.0f) {
				bool inShadow = false;
				// if the light has a limited cast range, don't test primitives
				// that are further from light for shadow test
				float maxCastDistance = currentLight->IsLightDistanceInfinite() ?
					std::numeric_limits<float>::max() : vectorMagn;
				// test shadow feeler if light supports it!
				if (currentLight->CastsShadows() &&
					ShadowFeelerIntersectsAnObject(Ray(intersectionPos+
						vectorToLight*SHADOW_FEELER_EPSILON, vectorToLight), 0.0f, maxCastDistance,
						primitiveToExclude)) {
					inShadow = true;
				}
				
				if (!inShadow) {
					intersectionResult.SetVectorToLight(vectorToLight);
					intersectionResult.SetIsVectorToLightNormalized(true);
					auto lightRadiance = currentLight->GetRadiance(intersectionResult, *this);

					Color lightRadColor4 = Color(lightRadiance[0], lightRadiance[1],
						lightRadiance[2], 0.0);
					newColor += isAreaLight ?
						primitiveMaterial->GetColorForAreaLight(intersectionResult)*
						lightRadColor4*projectionTerm*
						currentLight->GeometricTerm(intersectionResult)/
						currentLight->PDF(intersectionResult)
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
	for (unsigned int i = 0; i < numPrimitives; i++) {
		auto currentPrimitive = this->primitives[i];
		if (currentPrimitive == primitiveToExclude) {
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
