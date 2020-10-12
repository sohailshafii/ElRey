#include "InstancePrimitive.h"
#include <iostream>

InstancePrimitive::InstancePrimitive(std::string const & iName,
									 Primitive* primitive) :
	Primitive(iName), instancePrimitive(primitive) {
}

void InstancePrimitive::SamplePrimitive(Point3& resultingSample) {
	resultingSample = GetLocalToWorldPos(resultingSample);
	instancePrimitive->SamplePrimitive(resultingSample);
}

bool InstancePrimitive::HasBoundingBox() const {
	return instancePrimitive->HasBoundingBox();
}

AABBox InstancePrimitive::GetBoundingBox() const {
	AABBox localBoundingBox = instancePrimitive->GetBoundingBox();
	auto minPoint = localBoundingBox.GetMin();
	auto maxPoint = localBoundingBox.GetMax();
	return AABBox(GetLocalToWorldPos(minPoint),
				  GetLocalToWorldPos(maxPoint));
}
bool InstancePrimitive::Intersect(const Ray &rayWorld, float tMin,
								  float& tMax,
								  IntersectionResult &intersectionResult) {
	Ray rayToCast = rayWorld;
	Vector3 originalDir = rayWorld.GetDirection();
	Point3 originalOrigin = rayWorld.GetOrigin();
	rayToCast.SetOrigin(GetWorldToLocalPos(originalOrigin));
	Vector3 newDirection = GetWorldToLocalDir(originalDir);
	Vector3 newDirectionNorm = newDirection.Normalized();
	rayToCast.SetDirection(newDirectionNorm);
	bool found = instancePrimitive->Intersect(rayToCast, tMin, tMax, intersectionResult);
	if (found) {
		cachedPos = rayToCast.GetPositionAtParam(tMax);
		cachedDir = newDirectionNorm;
	}
	return found;
}

bool InstancePrimitive::IntersectShadow(const Ray &rayWorld,
										float tMin, float tMax) {
	Ray rayToCast = rayWorld;
	Vector3 originalDir = rayWorld.GetDirection();
	Point3 originalOrigin = rayWorld.GetOrigin();
	rayToCast.SetOrigin(GetWorldToLocalPos(originalOrigin));
	// TODO: fix, this screws up with scaling. ugh
	rayToCast.SetDirection(GetWorldToLocalDir(originalDir).Normalized());
	return instancePrimitive->IntersectShadow(rayToCast, tMin, tMax);
}

Vector3 InstancePrimitive::GetNormal(ParamsForNormal const &paramsForNormal) const {
	// hack; modify intersection position so that primitive thinks it's in local space
	ParamsForNormal resModified = paramsForNormal;
	resModified.SetIntersectionPosition(cachedPos);
	resModified.SetRayDirection(cachedDir);
	Vector3 normalLocal = instancePrimitive->GetNormal(resModified);
	
	return GetWorldToLocalTransposeDir(normalLocal).Normalized();
}

Vector3 InstancePrimitive::GetNormalAtPosition(Point3 const &position) const {
	Vector3 normalLocal = instancePrimitive->GetNormalAtPosition(
																 GetWorldToLocalPos(position));
	return GetWorldToLocalTransposeDir(normalLocal).Normalized();
}

void InstancePrimitive::SetLocalToWorld(Matrix4x4 const & localToWorld) {
	this->localToWorld = localToWorld;
	this->localToWorldTranspose = localToWorld.Transpose();
}

void InstancePrimitive::SetWorldToLocal(Matrix4x4 const & worldToLocal) {
	this->worldToLocal = worldToLocal;
	this->worldToLocalTranspose = worldToLocal.Transpose();
}

void InstancePrimitive::SetTransformAndInverse(Matrix4x4 const & localToWorld,
									   Matrix4x4 const & worldToLocal) {
	this->localToWorld = localToWorld;
	this->localToWorldTranspose = localToWorld.Transpose();
	this->worldToLocal = worldToLocal;
	this->worldToLocalTranspose = worldToLocal.Transpose();
}

Vector3 InstancePrimitive::GetLocalToWorldDir(Vector3 const & inDir) const {
	return localToWorld*inDir;
}

Vector3 InstancePrimitive::GetWorldToLocalDir(Vector3 const & inDir) const {
	return worldToLocal*inDir;
}

Vector3 InstancePrimitive::GetLocalToWorldTransposeDir(Vector3 const & inDir) const {
	return localToWorldTranspose*inDir;
}

Vector3 InstancePrimitive::GetWorldToLocalTransposeDir(Vector3 const & inDir) const {
	return worldToLocalTranspose*inDir;
}

Point3 InstancePrimitive::GetLocalToWorldPos(Point3 const & inPos) const {
	return localToWorld*inPos;
}

Point3 InstancePrimitive::GetWorldToLocalPos(Point3 const & inPos) const {
	return worldToLocal*inPos;
}
