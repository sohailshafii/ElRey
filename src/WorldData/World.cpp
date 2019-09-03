#include "World.h"
#include <stdexcept>
#include <cstring>

World::World() {
	primitives = nullptr;
}

World::World(Primitive **primitives, unsigned int numPrimitives) {
	this->primitives = primitives;
	this->numPrimitives = numPrimitives;
}

World::~World() {
	cleanUpPrimitives(this->primitives, this->numPrimitives);
}

void World::cleanUpPrimitives(Primitive **primitivesToClean, unsigned int
	numToClean) {
	if (primitivesToClean != nullptr) {
		for(unsigned int i = 0; i < numToClean; i++) {
			delete primitivesToClean[i];
		}
		delete [] primitivesToClean;
	}
}

void World::AddPrimitive(Primitive *newPrimitive) {
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

void World::AddPrimitives(Primitive **newPrimitives, unsigned int numNewPrimitives) {
	if (newPrimitives == nullptr || numNewPrimitives == 0) {
		throw std::runtime_error("Trying to add invalid primitives!");
	}

	if (this->primitives == nullptr) {
		this->numPrimitives = numNewPrimitives;
		this->primitives = newPrimitives;
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

bool World::Intersect(const Ray &ray, Color &newColor,
	float tMin, float& tMax) {
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
