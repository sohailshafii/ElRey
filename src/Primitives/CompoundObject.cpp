#include "CompoundObject.h"

bool CompoundObject::Intersect(const Ray &ray, float tMin, float& tMax,
			   IntersectionResult &intersectionResult) {
	unsigned int numElements = primitives.size();
	closestPrimSoFar = nullptr;
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		
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

Vector3 CompoundObject::GetNormalAtPosition(
	IntersectionResult const &intersectionResult) const {
	Primitive* foundPrim = GetPrimitiveByIntersectionResult(intersectionResult);
	return foundPrim != nullptr ? foundPrim->GetNormalAtPosition(intersectionResult)
		: Vector3();
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

void CompoundObject::AddPrimitive(Primitive * primitive) {
	primitives.push_back(primitive);
	RecomputeTransformsForChildren();
}

void CompoundObject::RemovePrimitiveAtIndex(unsigned int index) {
	primitives.erase(primitives.begin() + index);
	RecomputeTransformsForChildren();
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
		RecomputeTransformsForChildren();
	}
}

void CompoundObject::RecomputeTransformsForChildren() {
	worldToLocalChildren.MakeIdentity();
	localToWorldChildren.MakeIdentity();
	worldToLocalTransposeChildren.MakeIdentity();
	
	for(auto const primitive : primitives) {
		worldToLocalChildren *= primitive->GetWorldToLocal();
		localToWorldChildren *= primitive->GetLocalToWorld();
		worldToLocalTransposeChildren *= primitive->GetLocalToWorldTranspose();
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


Vector3 CompoundObject::GetLocalToWorldDir(Vector3 const & inDir) const {
	return localToWorld*localToWorldChildren*inDir;
}

Vector3 CompoundObject::GetWorldToLocalDir(Vector3 const & inDir) const {
	return worldToLocal*worldToLocalChildren*inDir;
}

Vector3 CompoundObject::GetWorldToLocalTransposeDir(Vector3 const & inDir) const {
	return worldToLocalTranspose*worldToLocalTransposeChildren*inDir;
}

Point3 CompoundObject::GetLocalToWorldPos(Point3 const & inPos) const {
	return localToWorld*localToWorldChildren*inPos;
}

Point3 CompoundObject::GetWorldToLocalPos(Point3 const & inPos) const {
	return worldToLocal*worldToLocalChildren*inPos;
}

Point3 CompoundObject::GetWorldToLocalTransposePos(Point3 const & inPos) const {
	return worldToLocalTranspose*worldToLocalTransposeChildren*inPos;
}
