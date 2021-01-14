#include "CompoundObject.h"
#include "OpenCylinder.h"
#include <cassert>

Primitive* CompoundObject::Intersect(const Ray &ray, float tMin, float& tMax,
									 IntersectionResult &intersectionResult) {
	unsigned int numElements = primitives.size();
	Primitive* closestPrimSoFar = nullptr;

	Primitive *closestChildBeforeTests = intersectionResult.childPrimitiveHit;
	IntersectionResult intersectionResSoFar;
	
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		// reset intersection data in-between tests
		intersectionResSoFar.ResetPrimIntersectionData();
		auto hitPrim = currPrimitive->Intersect(ray, tMin, tMax, intersectionResSoFar);
		if (hitPrim != nullptr) {
			closestPrimSoFar = hitPrim;
		}
	}
	
	if (closestPrimSoFar != nullptr) {
		// if any compound objects did not set a child primitive that
		// return an intersection, set it. this means if we have a tree of
		// of compound objects, we store the intersection with the deepest one
		// we need this because if we have instance objects in the tree, instance
		// primitives will return themselves as the closest intersection hit (so that
		// they can apply transformations for lighting after the fact, etc). an instance
		// when call our sampler, normal, etc functions and we need to know
		// which part of compound object was originally hit
		if (closestChildBeforeTests == intersectionResSoFar.childPrimitiveHit) {
			intersectionResSoFar.childPrimitiveHit = closestPrimSoFar;
		}
		intersectionResult = intersectionResSoFar;
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
	
	// if there is an instance primitive above us it will effectively tell calling
	// code that it was the object that was hit. that's ok, because shadows
	// only care if a primitive was hit, and don't use that primitive for additional
	// processing
	return hitPrimitive;
}

Vector3 CompoundObject::GetNormal(ParamsForNormal const &paramsForNormal) const {
	return paramsForNormal.childPrimHit != nullptr ?
		paramsForNormal.childPrimHit->GetNormal(paramsForNormal) : Vector3::Zero();
}

// not valid for this primitive
Vector3 CompoundObject::ComputeHardNormal(Point3 const &position) const {
	return Vector3::Zero();
}

void CompoundObject::SamplePrimitive(Point3& resultingSample,
									 IntersectionResult const & intersectionResult) {
	auto childPrim = intersectionResult.childPrimitiveHit;
	if (childPrim != nullptr) {
		childPrim->SamplePrimitive(resultingSample, intersectionResult);
	}
}

const GenericSampler* CompoundObject::GetSampler(IntersectionResult const & intersectionResult) {
	return intersectionResult.childPrimitiveHit != nullptr ?
		intersectionResult.childPrimitiveHit->GetSampler(intersectionResult) : nullptr;
}

float CompoundObject::PDF(ParamsForNormal const &paramsForNormal) const {
	return paramsForNormal.childPrimHit != nullptr ?
		paramsForNormal.childPrimHit->PDF(paramsForNormal) : 0.0f;
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
