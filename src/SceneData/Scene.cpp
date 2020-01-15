#include "Scene.h"
#include <stdexcept>
#include <cstring>

Scene::Scene() {
	primitives = nullptr;
	numPrimitives = 0;

	lights = nullptr;
	numLights = 0;
}

Scene::Scene(Primitive **primitives, unsigned int numPrimitives) {
	this->primitives = primitives;
	this->numPrimitives = numPrimitives;
}

Scene::~Scene() {
	cleanUpPrimitives(this->primitives, this->numPrimitives);
	cleanUpLights(this->lights, this->numLights);
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
	bool struckPrimitive = false;
	
	for (unsigned int i = 0; i < numPrimitives; i++) {
		auto hitPrimitive = this->primitives[i]->Intersect(ray, newColor,
			tMin, tMax);
		if (hitPrimitive) {
			struckPrimitive = true;
		}
	}

	return struckPrimitive;
}
