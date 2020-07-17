#include "CompoundObject.h"

bool CompoundObject::Intersect(const Ray &ray, float tMin, float& tMax,
			   IntersectionResult &intersectionResult) {
	unsigned int numElements = primitives.size();
	bool hitSomething = false;
	Primitive* closestPrimitive = nullptr;
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		hitSomething =
			currPrimitive->Intersect(ray, tMin, tMax, intersectionResult);
		
		if (hitSomething) {
			closestPrimitive = currPrimitive;
		}
	}
	
	if (closestPrimitive != nullptr) {
		intersectionResult.SetPrimitiveName(closestPrimitive->GetName());
	}
	
	return hitSomething;
}

bool CompoundObject::IntersectShadow(const Ray &ray, float tMin,
									 float tMax) {
	unsigned int numElements = primitives.size();
	bool hitSomething = false;
	
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		hitSomething =
			currPrimitive->IntersectShadow(ray, tMin, tMax);
	}
	
	return hitSomething;
}

Vector3 CompoundObject::GetNormalAtPosition(
	IntersectionResult const &intersectionResult) const {
	std::string primName = intersectionResult.GetPrimitiveName();
	for(Primitive* currPrim : primitives) {
		if (currPrim->GetName() == intersectionResult.GetPrimitiveName()) {
			return currPrim->GetNormalAtPosition(intersectionResult);
		}
	}
	
	return Vector3();
}

void CompoundObject::SamplePrimitive(Point3& resultingSample) {
	// nothing to see here
}

float CompoundObject::PDF(const IntersectionResult& intersectionResult) const {
	return 0.0f;
	// nothing to see here
}

void CompoundObject::AddPrimitive(Primitive * primitive) {
	primitives.push_back(primitive);
}

void CompoundObject::RemovePrimitiveAtIndex(unsigned int index) {
	primitives.erase(primitives.begin() + index);
}

void CompoundObject::RemovePrimitiveWithName(std::string const & name) {
	unsigned int indexToRemove;
	bool foundObject;
	unsigned int numElements = primitives.size();
	for(unsigned int index = 0; index < numElements; index++) {
		if (primitives[index]->GetName() == name) {
			foundObject = true;
			indexToRemove = index;
		}
	}
	
	if (foundObject) {
		RemovePrimitiveAtIndex(indexToRemove);
	}
}
