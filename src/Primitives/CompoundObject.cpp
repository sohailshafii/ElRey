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
	
	if (closestPrimSoFar != nullptr) {
		intersectionResult.compoundPrimitiveToIntersectedPrim[this] =
			closestPrimSoFar;
	}
	
	return closestPrimSoFar;
}

bool CompoundObject::IntersectShadow(const Ray &ray, float tMin,
									 float tMax) {
	if (ignoreShadowTest) {
		return false;
	}
	unsigned int numElements = primitives.size();
	Primitive* hitPrimitive = nullptr;
	
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		if (currPrimitive->IntersectShadow(ray, tMin, tMax)) {
			return true;
		}
	}
	
	return false;
}

Vector3 CompoundObject::GetNormal(const ShadingInfo& shadingInfo) const {
	Primitive* childPrim = (*shadingInfo.compoundPrimitiveToIntersectedPrim)[(Primitive*)this];
	return childPrim != nullptr ?
		childPrim->GetNormal(shadingInfo) : Vector3::Zero();
}

Material const * CompoundObject::GetMaterial(const ShadingInfo& shadingInfo) {
	Primitive* childPrim = (*shadingInfo.compoundPrimitiveToIntersectedPrim)[(Primitive*)this];
	return childPrim != nullptr ?
		childPrim->GetMaterial(shadingInfo) : material.get();
}

// not valid for this primitive
Vector3 CompoundObject::ComputeHardNormal(Point3 const &position) const {
	return Vector3::Zero();
}

void CompoundObject::SamplePrimitive(Point3& resultingSample,
									 const ShadingInfo& shadingInfo) {
	Primitive* childPrim = (*shadingInfo.compoundPrimitiveToIntersectedPrim)[(Primitive*)this];
	if (childPrim != nullptr) {
		childPrim->SamplePrimitive(resultingSample, shadingInfo);
	}
}

const GenericSampler* CompoundObject::GetSampler(const ShadingInfo& shadingInfo) {
	Primitive* childPrim = (*shadingInfo.compoundPrimitiveToIntersectedPrim)[(Primitive*)this];
	return childPrim != nullptr ? childPrim->GetSampler(shadingInfo) : nullptr;
}

float CompoundObject::PDF(const ShadingInfo& shadingInfo) const {
	Primitive* childPrim = (*shadingInfo.compoundPrimitiveToIntersectedPrim)[(Primitive*)this];
	return childPrim != nullptr ? childPrim->PDF(shadingInfo) : 0.0f;
}

AABBox CompoundObject::GetBoundingBox() const {
	return boundingBox;
}

void CompoundObject::AddPrimitive(std::shared_ptr<Primitive> primitive) {
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

std::shared_ptr<Primitive> CompoundObject::GetSubPrimitiveByName(std::string const & intersecPrimName) const {
	for (auto primitive : primitives) {
		if (primitive->GetName() == intersecPrimName) {
			return primitive;
		}
	}
	
	return nullptr;
}
