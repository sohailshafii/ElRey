#include "CompoundObject.h"
#include "OpenCylinder.h"
#include <cassert>

Primitive* CompoundObject::Intersect(const Ray &ray, float tMin, float& tMax,
									 IntersectionResult &intersectionResult) {
	unsigned int numElements = primitives.size();
	Primitive* closestPrimSoFar = nullptr;

	Primitive *closestChildBeforeTests = intersectionResult.childPrimitiveHit;
	
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		auto hitPrim = currPrimitive->Intersect(ray, tMin, tMax, intersectionResult);
		if (hitPrim != nullptr) {
			closestPrimSoFar = hitPrim;
		}
	}
	
	if (closestPrimSoFar != nullptr) {
		bool childPrimitiveUnchangedAfterTests =
			intersectionResult.childPrimitiveHit == closestChildBeforeTests;
		// if child primitive has not changed,
		// set it. so that we set deepest intersection hit
		if (childPrimitiveUnchangedAfterTests) {
			intersectionResult.childPrimitiveHit = closestPrimSoFar;
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
	
	// if there is an instance primitive above us it will effectively tell calling
	// code that it was the object that was hit. that's ok, because shadows
	// only care if a primitive was hit, and don't use that primitive for additional
	// processing
	return hitPrimitive;
}

Vector3 CompoundObject::GetNormal(const ShadingInfo& shadingInfo) const {
	return shadingInfo.childPrimitiveHit != nullptr ?
		shadingInfo.childPrimitiveHit->GetNormal(shadingInfo) : Vector3::Zero();
}

// not valid for this primitive
Vector3 CompoundObject::ComputeHardNormal(Point3 const &position) const {
	return Vector3::Zero();
}

void CompoundObject::SamplePrimitive(Point3& resultingSample,
									 const ShadingInfo& shadingInfo) {
	auto childPrim = shadingInfo.childPrimitiveHit;
	if (childPrim != nullptr) {
		childPrim->SamplePrimitive(resultingSample, shadingInfo);
	}
}

const GenericSampler* CompoundObject::GetSampler(const ShadingInfo& shadingInfo) {
	return shadingInfo.childPrimitiveHit != nullptr ?
		shadingInfo.childPrimitiveHit->GetSampler(shadingInfo) : nullptr;
}

float CompoundObject::PDF(const ShadingInfo& shadingInfo) const {
	return shadingInfo.childPrimitiveHit != nullptr ?
		shadingInfo.childPrimitiveHit->PDF(shadingInfo) : 0.0f;
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
