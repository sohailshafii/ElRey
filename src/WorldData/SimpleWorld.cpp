#include "SimpleWorld.h"
#include "SceneData/IntersectionResult.h"
#include "Primitives/Primitive.h"
#include <sstream>
#include <stdexcept>

SimpleWorld::SimpleWorld() {
	
}

SimpleWorld::SimpleWorld(std::shared_ptr<Primitive> *primitives,
						 unsigned int numPrimitives) {
	for (unsigned int i = 0; i < numPrimitives; i++) {
		this->primitives.push_back(primitives[i]);
	}
}

SimpleWorld::~SimpleWorld() {
	primitives.clear();
}

void SimpleWorld::AddPrimitive(std::shared_ptr<Primitive> const & newPrimitive) {
	if (newPrimitive == nullptr) {
		throw std::runtime_error("Trying to add invalid primitive!");
	}
	std::string const & primitiveName = newPrimitive->GetName();
	for(auto prim : primitives)
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

void SimpleWorld::AddPrimitives(std::shared_ptr<Primitive> *newPrimitives, unsigned int numNewPrimitives) {
	if (newPrimitives == nullptr || numNewPrimitives == 0) {
		throw std::runtime_error("Trying to add invalid primitives!");
	}

	for (unsigned int i = 0; i < numNewPrimitives; i++) {
		this->primitives.push_back(newPrimitives[i]);
	}
}

void SimpleWorld::AddPrimitives(std::vector<std::shared_ptr<Primitive>> const & newPrimitives) {
	size_t numNewPrimitives = newPrimitives.size();
	if (numNewPrimitives == 0) {
		throw std::runtime_error("Trying to add invalid primitives!");
	}

	for (unsigned int i = 0; i < numNewPrimitives; i++) {
		this->primitives.push_back(newPrimitives[i]);
	}
}

void SimpleWorld::RemovePrimitive(std::shared_ptr<Primitive> const & primitiveToRemove) {
	if (primitiveToRemove == nullptr) {
		throw std::runtime_error("Trying to remove invalid primitive!");
	}
	std::string const & primitiveName = primitiveToRemove->GetName();
	RemovePrimitiveByName(primitiveName);
}

void SimpleWorld::RemovePrimitiveByName(std::string const & name) {
	for (auto it = primitives.begin(); it != primitives.end(); ) {
		if ((*it)->GetName() == name) {
			primitives.erase(it);
			break;
		} else {
			++it;
		}
	}
}

std::shared_ptr<Primitive> SimpleWorld::GetPrimitive(unsigned int index) {
	return primitives[index];
}

std::shared_ptr<Primitive> SimpleWorld::FindPrimitiveByName(const std::string& name) {
	std::shared_ptr<Primitive> foundPrimitive = nullptr;

	for (auto currentPrimitive : primitives) {
		if (currentPrimitive->GetName() == name) {
			foundPrimitive = currentPrimitive;
			break;
		}
		// also search primitive itself, it might have children
		auto foundPrim = currentPrimitive->GetSubPrimitiveByName(name);
		if (foundPrim != nullptr) {
			foundPrimitive = foundPrim;
			break;
		}
	}

	return foundPrimitive;
}

Primitive* SimpleWorld::Intersect(const Ray &ray, float tMin, float& tMax,
							IntersectionResult &intersectionResult) {
	Primitive* closestPrimitive = nullptr;
	for (auto currentPrimitive : primitives) {
		auto hitPrimitive = currentPrimitive->Intersect(ray, tMin, tMax,
														intersectionResult);
		if (hitPrimitive != nullptr) {
			closestPrimitive = hitPrimitive;
		}
	}

	return closestPrimitive;
}

bool SimpleWorld::ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
													   float tMax) {
	Ray rayToCast = ray;
	Vector3 originalDir = ray.GetDirection();
	Point3 originalOrigin = ray.GetOrigin();
	for (auto currentPrimitive : primitives) {
		if (currentPrimitive->IntersectShadow(ray, tMin, tMax)) {
			return true;
		}
	}
	return false;
}
