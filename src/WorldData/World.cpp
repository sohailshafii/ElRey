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
	if (primitives != nullptr) {
		for(int i = 0; i < numPrimitives; i++) {
			delete this->primitives[i];
		}
		delete [] this->primitives;
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
		delete [] oldPrimitives;
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
		delete oldPrimitives;
	}
}
