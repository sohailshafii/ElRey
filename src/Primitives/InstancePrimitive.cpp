#include "InstancePrimitive.h"

InstancePrimitive::InstancePrimitive(std::string const & iName,
									 Primitive* primitive) :
	Primitive(iName), instancePrimitive(primitive) {
}

void InstancePrimitive::SamplePrimitiveLocal(Point3& resultingSample) {
	instancePrimitive->SamplePrimitiveLocal(resultingSample);
}

void InstancePrimitive::SamplePrimitiveWorld(Point3& resultingSample) {
	instancePrimitive->SamplePrimitiveWorld(resultingSample);
}

bool InstancePrimitive::HasBoundingBox() const {
	return instancePrimitive->HasBoundingBox();
}

AABBox InstancePrimitive::GetBoundingBoxLocal() const {
	return instancePrimitive->GetBoundingBoxLocal();
}

AABBox InstancePrimitive::GetBoundingBoxWorld() const {
	return instancePrimitive->GetBoundingBoxWorld();
}

bool InstancePrimitive::Intersect(const Ray &rayWorld, float tMin,
								  float& tMax,
								  IntersectionResult &intersectionResult) {
	Ray rayToCast = rayWorld;
	Vector3 originalDir = rayWorld.GetDirection();
	Point3 originalOrigin = rayWorld.GetOrigin();
	if (isTransformed) {
		rayToCast.SetOrigin(GetWorldToLocalPos(originalOrigin));
		rayToCast.SetDirection(GetWorldToLocalDir(originalDir));
	}
	return instancePrimitive->Intersect(rayToCast, tMin, tMax, intersectionResult);
}

bool InstancePrimitive::IntersectShadow(const Ray &rayWorld,
										float tMin, float tMax) {
	Ray rayToCast = rayWorld;
	Vector3 originalDir = rayWorld.GetDirection();
	Point3 originalOrigin = rayWorld.GetOrigin();
	if (isTransformed) {
		rayToCast.SetOrigin(GetWorldToLocalPos(originalOrigin));
		rayToCast.SetDirection(GetWorldToLocalDir(originalDir));
	}
	return instancePrimitive->IntersectShadow(rayToCast, tMin, tMax);
}

Vector3 InstancePrimitive::GetNormal(IntersectionResult const &intersectionResult)
	const {
	Vector3 normalLocal = instancePrimitive->GetNormal(intersectionResult);
	return isTransformed ? GetWorldToLocalTransposeDir(normalLocal).Normalized() :
		normalLocal;
}

// TODO: re-write to use transforms in one place when ready
// primitive base class won't do any transformations but provide overridable pure
// functions for intersect, intersect shadow, and normal calculation
bool InstancePrimitive::IntersectLocal(const Ray &rayLocal,
									   float tMin, float& tMax,
									   IntersectionResult &intersectionResult) {
	return instancePrimitive->Intersect(rayLocal, tMin, tMax, intersectionResult);
}

bool InstancePrimitive::IntersectShadowLocal(const Ray &rayLocal,
											 float tMin, float tMax)  {
	return instancePrimitive->IntersectShadow(rayLocal, tMin, tMax);
}

Vector3 InstancePrimitive::GetNormalLocal(IntersectionResult const
							   &intersectionResult) const {
	return instancePrimitive->GetNormal(intersectionResult);
}
