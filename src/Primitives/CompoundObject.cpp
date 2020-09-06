#include "CompoundObject.h"
#include "OpenCylinder.h"

// TODO: what if compound object contains compound object? each object should transform the ray on
// its own. same goes for normals
bool CompoundObject::IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
									IntersectionResult &intersectionResult) {
	unsigned int numElements = primitives.size();
	closestPrimSoFar = nullptr;
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		
		if (currPrimitive->Intersect(rayLocal, tMin, tMax, intersectionResult)) {
			closestPrimSoFar = currPrimitive;
		}
	}
	
	if (closestPrimSoFar != nullptr) {
		intersectionResult.SetPrimitiveName(closestPrimSoFar->GetName());
		return true;
	}
	
	return false;
}

bool CompoundObject::IntersectShadowLocal(const Ray &rayLocal, float tMin,
										  float tMax) {
	unsigned int numElements = primitives.size();
	bool hitSomething = false;
	
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		hitSomething =
			currPrimitive->IntersectShadow(rayLocal, tMin, tMax);
	}
	
	return hitSomething;
}

Vector3 CompoundObject::GetNormalLocal(IntersectionResult const &intersectionResult) const {
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

void CompoundObject::SamplePrimitiveLocal(Point3& resultingSample) {
	if (closestPrimSoFar != nullptr) {
		closestPrimSoFar->SamplePrimitiveLocal(resultingSample);
	}
}

void CompoundObject::SamplePrimitiveWorld(Point3& resultingSample) {
	if (closestPrimSoFar != nullptr) {
		closestPrimSoFar->SamplePrimitiveWorld(resultingSample);
	}
}

float CompoundObject::PDF(const IntersectionResult& intersectionResult) const {
	Primitive* foundPrim = GetPrimitiveByIntersectionResult(intersectionResult);
	return foundPrim != nullptr ? foundPrim->PDF(intersectionResult) : 0.0f;
}

AABBox CompoundObject::GetBoundingBoxLocal() const {
	return localBoundingBox;
}

AABBox CompoundObject::GetBoundingBoxWorld() const {
	auto minPoint = localBoundingBox.GetMin();
	auto maxPoint = localBoundingBox.GetMax();
	return AABBox(GetLocalToWorldPos(minPoint),
				  GetLocalToWorldPos(maxPoint));
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
	localBoundingBox.Reset();
	unsigned int numElements = primitives.size();
	for(unsigned int index = 0; index < numElements; index++) {
		localBoundingBox.Superset(primitives[index]->GetBoundingBoxLocal());
	}
}
