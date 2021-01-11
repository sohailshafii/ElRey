#include "Sphere.h"
#include "Math/Vector3.h"
#include "Math/CommonMath.h"
#include "SceneData/IntersectionResult.h"
#include <cmath>
#include <iostream>

Primitive* Sphere::Intersect(const Ray &ray, float tMin, float& tMax,
							 IntersectionResult &intersectionResult) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();

	Vector3 centerToRayOrigin = rayOrigin - center;
	float a = rayDirection*rayDirection;
	float b = 2.0f * (centerToRayOrigin * rayDirection);
	float c = centerToRayOrigin*centerToRayOrigin - radiusSqr;
	float discr = b*b - 4.0f*a*c;

	if (discr < 0.0f) {
		return nullptr;
	}

	float e = sqrt(discr);
	float denom = 2.0f*a;
	float t = (-b-e)/denom;
	// smaller root
	if (t > EPSILON && t > tMin && t < tMax) {
		tMax = t;
		intersectionResult.SetIntersectionT(tMax);
		return this;
	}

	t = (-b+e)/denom;
	if (t > EPSILON && t > tMin && t < tMax) {
		tMax = t;
		intersectionResult.SetIntersectionT(tMax);
		return this;
	}

	// all tests failed so far
	return nullptr;
}

Primitive* Sphere::IntersectShadow(const Ray &ray, float tMin, float tMax) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();

	Vector3 centerToRayOrigin = rayOrigin - center;
	float a = rayDirection*rayDirection;
	float b = 2.0f * (centerToRayOrigin * rayDirection);
	float c = centerToRayOrigin*centerToRayOrigin - radiusSqr;
	float discr = b*b - 4.0f*a*c;

	if (discr < 0.0f) {
		return nullptr;
	}

	float e = sqrt(discr);
	float denom = 2.0f*a;
	float t = (-b-e)/denom;
	// smaller root
	if (t > EPSILON && t > tMin && t < tMax) {
		return this;
	}

	t = (-b+e)/denom;
	if (t > EPSILON && t > tMin && t < tMax) {
		return this;
	}
	
	return nullptr;
}

Vector3 Sphere::GetNormal(ParamsForNormal const &paramsForNormal) const {
	return ComputeHardNormal(paramsForNormal.intersectionPos);
}

Vector3 Sphere::ComputeHardNormal(Point3 const &position) const {
	Vector3 normalVec = (position - center);
	normalVec.Normalize();
	return normalVec;
}

void Sphere::SamplePrimitive(Point3& resultingSample,
							 IntersectionResult const & intersectionResult) {
	// Not valid; necessary for sampling if we want area lights that are spheres
}

float Sphere::PDF(ParamsForNormal const &paramsForNormal) const {
	return 1.0f; // invalid until we need to use it
}

AABBox Sphere::GetBoundingBox() const {
	return boundingBoxLocal;
}

AABBox Sphere::ComputeBoundingBoxLocal() const {
	Point3 minPoint = center - Vector3(radius, radius, radius);
	Point3 maxPoint = center + Vector3(radius, radius, radius);
	
	return AABBox(minPoint, maxPoint);
}
