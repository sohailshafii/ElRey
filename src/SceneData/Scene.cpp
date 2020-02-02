#include "Scene.h"
#include <stdexcept>
#include <cstring>
#include "Math/CommonMath.h"
#include "IntersectionResult.h"
#include "SceneData/DirectionalLight.h"

Scene::Scene() {
	primitives = nullptr;
	numPrimitives = 0;

	lights = nullptr;
	numLights = 0;
	mainCamera = nullptr;
}

Scene::Scene(Primitive **primitives, unsigned int numPrimitives) {
	this->primitives = primitives;
	this->numPrimitives = numPrimitives;
	mainCamera = nullptr;
}

Scene::~Scene() {
	cleanUpPrimitives(this->primitives, this->numPrimitives);
	cleanUpLights(this->lights, this->numLights);
	if (mainCamera != nullptr) {
		delete mainCamera;
	}
}

void Scene::cleanUpPrimitives(Primitive **primitivesToClean, unsigned int
	numToClean) {
	if (primitivesToClean != nullptr) {
		for(unsigned int i = 0; i < numToClean; i++) {
			delete primitivesToClean[i];
		}
		delete [] primitivesToClean;
	}
}

void Scene::cleanUpLights(Light** lightsToClean, unsigned int numToClean) {
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

bool Scene::Intersect(const Ray &ray, Color &newColor,
	float tMin, float& tMax) const {
	
	Primitive* closestPrimitive = nullptr;
	for (unsigned int i = 0; i < numPrimitives; i++) {
		auto currentPrimitive = this->primitives[i];
		auto hitPrimitive = this->primitives[i]->Intersect(ray, tMin, tMax);
		if (hitPrimitive) {
			closestPrimitive = currentPrimitive;
		}
	}
	
	if (closestPrimitive != nullptr)
	{
		std::shared_ptr<Material> primitivePtr = closestPrimitive->GetMaterial();
		IntersectionResult interRes(ray, Vector3(0.0, 0.0, 0.0), tMax);
	
		newColor = primitivePtr->GetAmbientColor(interRes);
		for (unsigned int i = 0; i < numLights; i++) {
			auto currentLight = this->lights[i];
			auto lightRadiance = currentLight->GetRadiance();
			Color lightRadColor4 = Color(lightRadiance[0], lightRadiance[1],
										 lightRadiance[2], 0.0);
			auto intersectionPos = ray.GetPositionAtParam(tMax);
			Vector3 vectorToLight = -currentLight->GetDirectionFromPosition(
																		   intersectionPos);
			// skip ambient lights, or lights with zero magnitude
			// TODO: remove ambient light type?
			float vectorMagn = vectorToLight.Norm();
			if (vectorMagn < EPSILON) {
				continue;
			}
			
			vectorToLight.Normalize();
			Vector3 normalVec = closestPrimitive->GetNormalAtPosition(intersectionPos);
			float projectionTerm = vectorToLight*normalVec;
			if (projectionTerm > 0.0f) {
				interRes.SetVectorToLight(vectorToLight);
				newColor += primitivePtr->GetDirectColor(interRes)*lightRadColor4*projectionTerm;
			}
		}
	}

	return closestPrimitive != nullptr;
}

void Scene::TranslateAndRotate(const Vector3& translation, float rightRotationDegrees,
						float upRotationDegrees)
{
	mainCamera->TranslateAndRotate(translation, rightRotationDegrees, upRotationDegrees);
}

void Scene::CastIntoScene(unsigned char* pixels, unsigned int bytesPerPixel, float frameTime)
{
	mainCamera->CastIntoScene(pixels, bytesPerPixel, this, frameTime);
}
