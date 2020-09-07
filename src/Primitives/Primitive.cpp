#include "Primitive.h"

bool Primitive::Intersect(const Ray &rayWorld, float tMin, float& tMax,
	IntersectionResult &intersectionResult) {
	Ray rayToCast = rayWorld;
	Vector3 originalDir = rayWorld.GetDirection();
	Point3 originalOrigin = rayWorld.GetOrigin();
	if (isTransformed) {
		rayToCast.SetOrigin(GetWorldToLocalPos(originalOrigin));
		rayToCast.SetDirection(GetWorldToLocalDir(originalDir));
	}
	return IntersectLocal(rayToCast, tMin, tMax, intersectionResult);
}

bool Primitive::IntersectShadow(const Ray &rayWorld, float tMin, float tMax) {
	Ray rayToCast = rayWorld;
	Vector3 originalDir = rayWorld.GetDirection();
	Point3 originalOrigin = rayWorld.GetOrigin();
	if (isTransformed) {
		rayToCast.SetOrigin(GetWorldToLocalPos(originalOrigin));
		rayToCast.SetDirection(GetWorldToLocalDir(originalDir));
	}
	return IntersectShadowLocal(rayToCast, tMin, tMax);
}

Vector3 Primitive::GetNormal(IntersectionResult const &intersectionResult) const {
	Vector3 normalLocal = GetNormalLocal(intersectionResult);
	return isTransformed ? GetWorldToLocalTransposeDir(normalLocal).Normalized() : normalLocal;
}

void Primitive::SetLocalToWorld(Matrix4x4 const & localToWorld) {
	this->localToWorld = localToWorld;
	this->localToWorldTranspose = localToWorld.Transpose();
	isTransformed = true;
}

void Primitive::SetWorldToLocal(Matrix4x4 const & worldToLocal) {
	this->worldToLocal = worldToLocal;
	this->worldToLocalTranspose = worldToLocal.Transpose();
	isTransformed = true;
}

void Primitive::SetTransformAndInverse(Matrix4x4 const & localToWorld,
									   Matrix4x4 const & worldToLocal) {
	this->localToWorld = localToWorld;
	this->localToWorldTranspose = localToWorld.Transpose();
	this->worldToLocal = worldToLocal;
	this->worldToLocalTranspose = worldToLocal.Transpose();
	isTransformed = true;
}

Vector3 Primitive::GetLocalToWorldDir(Vector3 const & inDir) const {
	return localToWorld*inDir;
}

Vector3 Primitive::GetWorldToLocalDir(Vector3 const & inDir) const {
	return worldToLocal*inDir;
}

Vector3 Primitive::GetLocalToWorldTransposeDir(Vector3 const & inDir) const {
	return localToWorldTranspose*inDir;
}

Vector3 Primitive::GetWorldToLocalTransposeDir(Vector3 const & inDir) const {
	return worldToLocalTranspose*inDir;
}

Point3 Primitive::GetLocalToWorldPos(Point3 const & inPos) const {
	return localToWorld*inPos;
}

Point3 Primitive::GetWorldToLocalPos(Point3 const & inPos) const {
	return worldToLocal*inPos;
}

void Primitive::SamplePrimitiveWorld(Point3& resultingSample) {
	SamplePrimitiveLocal(resultingSample);
	resultingSample = GetLocalToWorldPos(resultingSample);
}
