#include "BaseAccelerator.h"
#include "Primitives/Primitive.h"
#include <sstream>
#include <stdexcept>

BaseAccelerator::BaseAccelerator(Primitive **primitives,
								 unsigned int numPrimitives) {
	for (unsigned int i = 0; i < numPrimitives; i++) {
		this->primitives.push_back(primitives[i]);
	}
}

BaseAccelerator::~BaseAccelerator() {
	for(auto primitive : primitives) {
		delete primitive;
	}
	primitives.clear();
}

void BaseAccelerator::AddPrimitive(Primitive *newPrimitive) {
	if (newPrimitive == nullptr) {
		throw std::runtime_error("Trying to add invalid primitive!");
	}
	std::string const & primitiveName = newPrimitive->GetName();
	for(Primitive* prim : primitives)
	{
		if (prim->GetName() == primitiveName) {
			std::stringstream exceptionMsg;
			exceptionMsg << "Trying to add primitive with duplicate name: " <<
				primitiveName << ". Primitive should have unique names otherwise " <<
				"compound objects cannot distinguish between children for normal " <<
				"vector calculation.\n";
			throw exceptionMsg;
		}
	}

	primitives.push_back(newPrimitive);
}

void BaseAccelerator::AddPrimitives(Primitive **newPrimitives, unsigned int numNewPrimitives) {
	if (newPrimitives == nullptr || numNewPrimitives == 0) {
		throw std::runtime_error("Trying to add invalid primitives!");
	}

	for (unsigned int i = 0; i < numNewPrimitives; i++) {
		this->primitives.push_back(newPrimitives[i]);
	}
}

void BaseAccelerator::AddPrimitives(std::vector<Primitive*> newPrimitives) {
	size_t numNewPrimitives = newPrimitives.size();
	if (numNewPrimitives == 0) {
		throw std::runtime_error("Trying to add invalid primitives!");
	}

	for (unsigned int i = 0; i < numNewPrimitives; i++) {
		this->primitives.push_back(newPrimitives[i]);
	}
}

void BaseAccelerator::RemovePrimitive(Primitive* primitiveToRemove) {
	if (primitiveToRemove == nullptr) {
		throw std::runtime_error("Trying to remove invalid primitive!");
	}
	std::string const & primitiveName = primitiveToRemove->GetName();
	RemovePrimitiveByName(primitiveName);
}

void BaseAccelerator::RemovePrimitiveByName(std::string const & name) {
	for (auto it = primitives.begin(); it != primitives.end(); ) {
		if ((*it)->GetName() == name) {
			primitives.erase(it);
			break;
		} else {
			++it;
		}
	}
}

Primitive* BaseAccelerator::FindPrimitiveByName(const std::string& name) {
	Primitive* foundPrimitive = nullptr;

	for (auto currentPrimitive : primitives) {
		if (currentPrimitive->GetName() == name) {
			foundPrimitive = currentPrimitive;
		}
	}

	return foundPrimitive;
}


