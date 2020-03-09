#include "Plane.h"
#include "CommonMath.h"

bool Plane::Intersect(const Ray &ray, float tMin, float& tMax,
					  IntersectionResult &intersectionResult) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();

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

bool Plane::IntersectShadow(const Ray &ray, float tMin, float tMax)
{
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();

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
