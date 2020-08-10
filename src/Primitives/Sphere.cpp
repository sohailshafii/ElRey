#include "Sphere.h"
#include "Math/Vector3.h"
#include "Math/CommonMath.h"
#include "SceneData/IntersectionResult.h"
#include <cmath>
#include <iostream>

bool Sphere::IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
	IntersectionResult &intersectionResult) {
	const Point3& rayOrigin = rayLocal.GetOrigin();
	const Vector3& rayDirection = rayLocal.GetDirection();

	Vector3 centerToRayOrigin = rayOrigin - center;
	float a = rayDirection*rayDirection;
	float b = 2.0f * (centerToRayOrigin * rayDirection);
	float c = centerToRayOrigin*centerToRayOrigin - radiusSqr;
	float discr = b*b - 4.0f*a*c;

	if (discr < 0.0f) {
		return false;
	}

	float e = sqrt(discr);
	float denom = 2.0f*a;
	float t = (-b-e)/denom;
	// smaller root
	if (t > EPSILON && t > tMin && t < tMax) {
		tMax = t;
		intersectionResult.SetIntersectionT(tMax);
		return true;
	}

	t = (-b+e)/denom;
	if (t > EPSILON && t > tMin && t < tMax) {
		tMax = t;
		intersectionResult.SetIntersectionT(tMax);
		return true;
	}

	// all tests failed so far
	return false;
}

bool Sphere::IntersectShadowLocal(const Ray &rayLocal, float tMin, float tMax) {
	const Point3& rayOrigin = rayLocal.GetOrigin();
	const Vector3& rayDirection = rayLocal.GetDirection();

	Vector3 centerToRayOrigin = rayOrigin - center;
	float a = rayDirection*rayDirection;
	float b = 2.0f * (centerToRayOrigin * rayDirection);
	float c = centerToRayOrigin*centerToRayOrigin - radiusSqr;
	float discr = b*b - 4.0f*a*c;

	if (discr < 0.0f) {
		return false;
	}

	float e = sqrt(discr);
	float denom = 2.0f*a;
	float t = (-b-e)/denom;
	// smaller root
	if (t > EPSILON && t > tMin && t < tMax) {
		return true;
	}

	t = (-b+e)/denom;
	if (t > EPSILON && t > tMin && t < tMax) {
		return true;
	}
	
	return false;
}

Vector3 Sphere::GetNormalWorld(IntersectionResult const &intersectionResult) const {
	Vector3 normalVec = (intersectionResult.GetIntersectionPosLocal() - center);
	normalVec.Normalize();
	return isTransformed ? GetWorldToLocalTransposeDir(normalVec) : normalVec;
}

void Sphere::SamplePrimitiveLocal(Point3& resultingSample) {
	// Not valid; necessary for sampling if we want area lights that are spheres
}

void Sphere::SamplePrimitiveWorld(Point3& resultingSample) {
	// Not valid; necessary for sampling if we want area lights that are spheres
}

float Sphere::PDF(const IntersectionResult& intersectionResult) const {
	return 1.0f; // invalid until we need to use it
}

AABBox Sphere::GetBoundingBoxLocal() const {
	return boundingBoxLocal;
}

AABBox Sphere::GetBoundingBoxWorld() const {
	Point3 worldSpaceMin =
		GetLocalToWorldPos(boundingBoxLocal.GetMin());
	Point3 worldSpaceMax =
		GetLocalToWorldPos(boundingBoxLocal.GetMax());
	return AABBox(worldSpaceMin[0], worldSpaceMin[1],
				  worldSpaceMin[2], worldSpaceMax[0],
				  worldSpaceMax[1], worldSpaceMax[2]);
}

AABBox Sphere::ComputeBoundingBoxLocal() const {
	Point3 minPoint = center - Vector3(radius, radius, radius);
	Point3 maxPoint = center + Vector3(radius, radius, radius);
	
	return AABBox(minPoint, maxPoint);
}
