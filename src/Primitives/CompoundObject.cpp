#include "CompoundObject.h"
#include "OpenCylinder.h"
#include <cassert>

Primitive* CompoundObject::Intersect(const Ray &ray, float tMin, float& tMax,
									 IntersectionResult &intersectionResult) {
	unsigned int numElements = primitives.size();
	Primitive* closestPrimSoFar = nullptr;
	
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		auto hitPrim = currPrimitive->Intersect(ray, tMin, tMax, intersectionResult);
		if (hitPrim != nullptr) {
			closestPrimSoFar = hitPrim;
		}
	}
	
	return closestPrimSoFar;
}

Primitive* CompoundObject::IntersectShadow(const Ray &ray, float tMin,
										   float tMax) {
	unsigned int numElements = primitives.size();
	Primitive* hitPrimitive = nullptr;
	
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		hitPrimitive = currPrimitive->IntersectShadow(ray, tMin, tMax);
		if (hitPrimitive != nullptr) {
			break;
		}
	}
	
	return hitPrimitive;
}

Vector3 CompoundObject::GetNormal(ParamsForNormal const &paramsForNormal) const {
	// not called directly
	return Vector3(0.0f, 0.0f, 0.0f);
}

// not valid for this primitive
Vector3 CompoundObject::ComputeHardNormal(Point3 const &position) const {
	return Vector3(0.0f, 0.0f, 0.0f);
}

void CompoundObject::SamplePrimitive(Point3& resultingSample,
									 IntersectionResult const & intersectionResult) {
	// not called directly
}

float CompoundObject::PDF(ParamsForNormal const &paramsForNormal) const {
	// not called directly
	return 0.0;
}

AABBox CompoundObject::GetBoundingBox() const {
	return boundingBox;
}

void CompoundObject::AddPrimitive(Primitive * primitive) {
	primitives.push_back(primitive);
	RecomputeBoundingBox();
}

void CompoundObject::RemovePrimitiveAtIndex(unsigned int index) {
	primitives.erase(primitives.begin() + index);
	RecomputeBoundingBox();
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
		RecomputeBoundingBox();
	}
}

void CompoundObject::RecomputeBoundingBox() {
	boundingBox.Reset();
	unsigned int numElements = primitives.size();
	for(unsigned int index = 0; index < numElements; index++) {
		auto primitiveBounds = primitives[index]->GetBoundingBox();
		boundingBox.Superset(primitiveBounds);
	}
}

void CompoundObject::PrintBounds() {
	boundingBox.PrintBounds();
}
