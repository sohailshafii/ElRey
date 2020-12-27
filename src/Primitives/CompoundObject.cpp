#include "CompoundObject.h"
#include "OpenCylinder.h"
#include <cassert>

bool CompoundObject::Intersect(const Ray &ray, float tMin, float& tMax,
							   IntersectionResult &intersectionResult) {
	unsigned int numElements = primitives.size();
	Primitive* closestPrimSoFar = nullptr;
	std::string primNameBeforeTestsForUs = intersectionResult.GetPrimitiveName();
	
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		if (currPrimitive->Intersect(ray, tMin, tMax, intersectionResult)) {
			closestPrimSoFar = currPrimitive;
		}
	}
	
	// set name of deepest intersection found so far
	if (closestPrimSoFar != nullptr) {
		std::string primNameAfterTestsForUs = intersectionResult.GetPrimitiveName();
		bool childrenDidNotSetPrimName = primNameBeforeTestsForUs == primNameAfterTestsForUs;
		// we know that we traversed deeper if if any children set
		// the prim name. if they have not, that means we are the only
		// compound object that set the closest intersection.
		if (childrenDidNotSetPrimName) {
			intersectionResult.SetPrimitiveName(closestPrimSoFar->GetName());
		}
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
		if (hitSomething) {
			break;
		}
	}
	
	return hitSomething;
}

Vector3 CompoundObject::GetNormal(ParamsForNormal const &paramsForNormal) const {
	Primitive* foundPrim = GetSubPrimitiveByName(paramsForNormal.GetPrimitiveName());
	Vector3 normalVec = foundPrim != nullptr ? foundPrim->GetNormal(paramsForNormal)
		: Vector3();
	return normalVec;
}

// not valid for this primitive
Vector3 CompoundObject::ComputeHardNormal(Point3 const &position) const {
	return Vector3(0.0f, 0.0f, 0.0f);
}

Primitive* CompoundObject::GetSubPrimitiveByName(std::string const & intersecPrimName) const {
	for (Primitive* currPrim : primitives) {
		if (currPrim->GetName() == intersecPrimName) {
			return currPrim;
		}
		else {
			auto* foundPrim = currPrim->GetSubPrimitiveByName(intersecPrimName);
			if (foundPrim != nullptr) {
				return foundPrim;
			}
		}
	}
	return nullptr;
}

void CompoundObject::SamplePrimitive(Point3& resultingSample,
									 IntersectionResult const & intersectionResult) {
	Primitive* foundPrim = GetSubPrimitiveByName(intersectionResult.GetPrimitiveName());
	if (foundPrim != nullptr) {
		foundPrim->SamplePrimitive(resultingSample, intersectionResult);
	}
}

float CompoundObject::PDF(ParamsForNormal const &paramsForNormal) const {
	Primitive* foundPrim = GetSubPrimitiveByName(paramsForNormal.GetPrimitiveName());
	return foundPrim != nullptr ? foundPrim->PDF(paramsForNormal) : 0.0f;
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

Material const * CompoundObject::GetMaterial(IntersectionResult const & intersectionResult) {
	Primitive* foundPrim = GetSubPrimitiveByName(intersectionResult.GetPrimitiveName());
	return foundPrim != nullptr ? foundPrim->GetMaterial(intersectionResult) : nullptr;
}

const GenericSampler* CompoundObject::GetSampler(IntersectionResult const & intersectionResult) {
	Primitive* foundPrim = GetSubPrimitiveByName(intersectionResult.GetPrimitiveName());
	return foundPrim != nullptr ?
	foundPrim->GetSampler(intersectionResult) : nullptr;
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
