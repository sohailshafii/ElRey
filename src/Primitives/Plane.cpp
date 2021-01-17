#include "Plane.h"
#include "CommonMath.h"

Primitive* Plane::Intersect(const Ray &ray, float tMin, float& tMax,
							IntersectionResult &intersectionResult) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();

	float vDotNormal = rayDirection*normal;
	if (vDotNormal > -EPSILON && vDotNormal <
		EPSILON) {
		return nullptr;
	}

	float t = -(rayOrigin*normal + distance)/vDotNormal;

	if (t < tMin || t > tMax) {
		return nullptr;
	}
	
	tMax = t;
	intersectionResult.rayIntersectT = tMax;
	return this;
}

Primitive* Plane::IntersectShadow(const Ray &ray, float tMin, float tMax)
{
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();

	float vDotNormal = rayDirection*normal;
	if (vDotNormal > -EPSILON && vDotNormal <
		EPSILON) {
		return nullptr;
	}

	float t = -(rayOrigin*normal + distance)/vDotNormal;

	if (t < tMin || t > tMax) {
		return nullptr;
	}
	
	return this;
}

Vector3 Plane::GetNormal(ParamsForNormal const &paramsForNormal) const {
	return normal;
}

Vector3 Plane::ComputeHardNormal(Point3 const &position) const {
	return normal;
}

void Plane::SamplePrimitive(Point3& resultingSample,
							IntersectionResult const & intersectionResult) {
	// not valid for area lighting
}

float Plane::PDF(ParamsForNormal const &paramsForNormal) const {
	return 1.0f; // not valid for area lighting
}
