#include "CompoundObject.h"
#include "OpenCylinder.h"

bool CompoundObject::Intersect(const Ray &ray, float tMin, float& tMax,
							   IntersectionResult &intersectionResult) {
	unsigned int numElements = primitives.size();
	closestPrimSoFar = nullptr;
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		// each primitive inside will do their own world-to-local transform
		// because this compound object should be treated as the coordinate system, or
		// world system, that its children live in
		if (currPrimitive->Intersect(ray, tMin, tMax, intersectionResult)) {
			closestPrimSoFar = currPrimitive;
		}
	}
	
	if (closestPrimSoFar != nullptr) {
		intersectionResult.SetPrimitiveName(closestPrimSoFar->GetName());
		return true;
	}
	
	return false;
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

Vector3 CompoundObject::GetNormal(IntersectionResult const &intersectionResult) const {
	Primitive* foundPrim = GetPrimitiveByIntersectionResult(intersectionResult);
	Vector3 normalVec = foundPrim != nullptr ? foundPrim->GetNormal(intersectionResult)
		: Vector3();
	return normalVec;
}

Primitive* CompoundObject::GetPrimitiveByIntersectionResult(IntersectionResult const &intersectionResult) const {
	auto intersecPrimName = intersectionResult.GetPrimitiveName();
	for (Primitive* currPrim : primitives) {
		if (currPrim->GetName() == intersecPrimName) {
			return currPrim;
		}
	}
	return nullptr;
}

void CompoundObject::SamplePrimitive(Point3& resultingSample) {
	if (closestPrimSoFar != nullptr) {
		closestPrimSoFar->SamplePrimitive(resultingSample);
	}
}

float CompoundObject::PDF(const IntersectionResult& intersectionResult) const {
	Primitive* foundPrim = GetPrimitiveByIntersectionResult(intersectionResult);
	return foundPrim != nullptr ? foundPrim->PDF(intersectionResult) : 0.0f;
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

std::shared_ptr<Material> CompoundObject::GetMaterial() {
	return closestPrimSoFar != nullptr ?
	 closestPrimSoFar->GetMaterial() : nullptr;
}

const GenericSampler* CompoundObject::GetSampler() {
	return closestPrimSoFar != nullptr ?
	 closestPrimSoFar->GetSampler() : nullptr;
}

void CompoundObject::RecomputeBoundingBox() {
	boundingBox.Reset();
	unsigned int numElements = primitives.size();
	for(unsigned int index = 0; index < numElements; index++) {
		boundingBox.Superset(primitives[index]->GetBoundingBox());
	}
}
