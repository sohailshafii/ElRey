#include "InstancePrimitive.h"

InstancePrimitive::InstancePrimitive(std::string const & iName,
									 Primitive* primitive) :
	Primitive(iName), instancePrimitive(primitive) {
}

void InstancePrimitive::SamplePrimitive(Point3& resultingSample) {
	// TODO: transform
	instancePrimitive->SamplePrimitive(resultingSample);
}

bool InstancePrimitive::HasBoundingBox() const {
	// TODO: transform
	return instancePrimitive->HasBoundingBox();
}

AABBox InstancePrimitive::GetBoundingBox() const {
	// TODO: transform
	return instancePrimitive->GetBoundingBox();
}
bool InstancePrimitive::Intersect(const Ray &rayWorld, float tMin,
								  float& tMax,
								  IntersectionResult &intersectionResult) {
	Ray rayToCast = rayWorld;
	Vector3 originalDir = rayWorld.GetDirection();
	Point3 originalOrigin = rayWorld.GetOrigin();
	// TODO: transform
	/*if (isTransformed) {
		rayToCast.SetOrigin(GetWorldToLocalPos(originalOrigin));
		rayToCast.SetDirection(GetWorldToLocalDir(originalDir));
	}*/
	return instancePrimitive->Intersect(rayToCast, tMin, tMax, intersectionResult);
}

bool InstancePrimitive::IntersectShadow(const Ray &rayWorld,
										float tMin, float tMax) {
	Ray rayToCast = rayWorld;
	Vector3 originalDir = rayWorld.GetDirection();
	Point3 originalOrigin = rayWorld.GetOrigin();
	// TODO: transform
	/*if (isTransformed) {
		rayToCast.SetOrigin(GetWorldToLocalPos(originalOrigin));
		rayToCast.SetDirection(GetWorldToLocalDir(originalDir));
	}*/
	return instancePrimitive->IntersectShadow(rayToCast, tMin, tMax);
}

Vector3 InstancePrimitive::GetNormal(IntersectionResult const &intersectionResult)
	const {
	Vector3 normalLocal = instancePrimitive->GetNormal(intersectionResult);
		
		// TODO: transform
	return //isTransformed ? GetWorldToLocalTransposeDir(normalLocal).Normalized() :
		normalLocal;
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
