#include "InstancePrimitive.h"

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
	rayToCast.SetDirection(GetWorldToLocalDir(originalDir));
	// TODO: make sure intersect ONLY touches certain things.
	// right now it doesn't enforce the rule that intersection position needs to be
	// set elsewhere. a primitive can modify it
	return instancePrimitive->Intersect(rayToCast, tMin, tMax, intersectionResult);
}

bool InstancePrimitive::IntersectShadow(const Ray &rayWorld,
										float tMin, float tMax) {
	Ray rayToCast = rayWorld;
	Vector3 originalDir = rayWorld.GetDirection();
	Point3 originalOrigin = rayWorld.GetOrigin();
	rayToCast.SetOrigin(GetWorldToLocalPos(originalOrigin));
	rayToCast.SetDirection(GetWorldToLocalDir(originalDir));
	return instancePrimitive->IntersectShadow(rayToCast, tMin, tMax);
}

Vector3 InstancePrimitive::GetNormal(IntersectionResult const &intersectionResult)
	const {
	// hack; modify intersection position so that primitive thinks its in local space
	IntersectionResult resModified = intersectionResult;
	resModified.SetIntersectionPosition(GetWorldToLocalPos(intersectionResult.GetIntersectionPos()));
	Vector3 normalLocal = instancePrimitive->GetNormal(resModified);

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
