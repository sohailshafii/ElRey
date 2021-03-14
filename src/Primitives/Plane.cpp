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

Vector3 Plane::GetNormal(const ShadingInfo& shadingInfo) const {
	return normal;
}

Vector3 Plane::ComputeHardNormal(Point3 const &position) const {
	return normal;
}

void Plane::SamplePrimitive(Point3& resultingSample,
							const ShadingInfo& shadingInfo) {
	// not valid for area lighting
}

float Plane::PDF(const ShadingInfo& shadingInfo) const {
	return 1.0f; // not valid for area lighting
}
