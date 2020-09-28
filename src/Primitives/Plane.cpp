#include "Plane.h"
#include "CommonMath.h"

bool Plane::Intersect(const Ray &rayLocal, float tMin, float& tMax,
					  IntersectionResult &intersectionResult) {
	const Point3& rayOrigin = rayLocal.GetOrigin();
	const Vector3& rayDirection = rayLocal.GetDirection();

	float vDotNormal = rayDirection*normal;
	if (vDotNormal > -EPSILON && vDotNormal <
		EPSILON) {
		return false;
	}

	float t = -(rayOrigin*normal + distance)/vDotNormal;

	if (t < tMin || t > tMax) {
		return false;
	}
	
	tMax = t;
	intersectionResult.SetIntersectionT(tMax);
	return true;
}

bool Plane::IntersectShadow(const Ray &rayLocal, float tMin, float tMax)
{
	const Point3& rayOrigin = rayLocal.GetOrigin();
	const Vector3& rayDirection = rayLocal.GetDirection();

	float vDotNormal = rayDirection*normal;
	if (vDotNormal > -EPSILON && vDotNormal <
		EPSILON) {
		return false;
	}

	float t = -(rayOrigin*normal + distance)/vDotNormal;

	if (t < tMin || t > tMax) {
		return false;
	}
	
	return true;
}

Vector3 Plane::GetNormal(IntersectionResult const &intersectionResult) const {
	return normal;
}

void Plane::SamplePrimitive(Point3& resultingSample) {
	// not valid for area lighting
}

float Plane::PDF(const IntersectionResult& intersectionResult) const {
	return 1.0f; // not valid for area lighting
}
