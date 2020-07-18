#include "OpenCylinder.h"
#include "Math/CommonMath.h"
#include "SceneData/IntersectionResult.h"
#include <cmath>
#include <algorithm>
#include <cstdlib>

void OpenCylinder::GenerateBoundingBox() {
	// TODO
}

bool OpenCylinder::Intersect(const Ray &ray, float tMin, float& tMax,
	IntersectionResult &intersectionResult) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();

	// TODO
	return false;
}

bool OpenCylinder::IntersectShadow(const Ray &ray, float tMin, float tMax) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();

	// TODO
	
	return false;
}

void OpenCylinder::SamplePrimitive(Point3& resultingSample) {
	// Leave out for now
}

float OpenCylinder::PDF(const IntersectionResult& intersectionResult) const {
	return 1.0f; // Doesn't return a valid value because we don't use it for sampling
}

AABBox OpenCylinder::GetBoundingBox() const {
	return boundingBox;
}

