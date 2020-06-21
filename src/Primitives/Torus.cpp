#include "Torus.h"
#include "CommonMath.h"
#include <algorithm>

bool Torus::Intersect(const Ray &ray, float tMin, float& tMax,
					IntersectionResult &intersectionResult) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	// TODO
	return false;
}

bool Torus::IntersectShadow(const Ray &ray, float tMin, float tMax)
{
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	// TODO
	return true;
}

void Torus::SamplePrimitive(Point3& resultingSample) {
	// nothing to see here for now
}

// each sample's probability is 1.0/inverseArea
float Torus::PDF(const IntersectionResult& intersectionResult) const {
	return 1.0f;
}

AABBox Torus::GetBoundingBox() const {
	return boundingBox;
}

