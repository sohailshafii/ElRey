#include "Disk.h"
#include "Math/CommonMath.h"
#include "SceneData/IntersectionResult.h"
#include <cmath>
#include <algorithm>
#include <cstdlib>

void Disk::GenerateBoundingBox() {
	// generate another vector on the plane
	Vector3 randomVector(0.0f, 1.0f, 0.0f);
	while (fabs(normalVec*randomVector) < EPSILON) {
		randomVector[0] =
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		randomVector[1] =
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		randomVector[2] =
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		randomVector.Normalize();
	}
	
	Vector3 vectorOnPlane = normalVec ^ randomVector;
	float radius = sqrt(radiusSquared);
	Point3 point1 = center + vectorOnPlane*radius;
	Point3 point2 = center - vectorOnPlane*radius;
	
	Point3 maxPoint(std::max(point1[0], point2[0]),
					std::max(point1[1], point2[1]),
					std::max(point1[2], point2[2]));
	Point3 minPoint(std::min(point1[0], point2[0]),
					std::min(point1[1], point2[1]),
					std::min(point1[2], point2[2]));
	boundingBoxLocal = AABBox(minPoint, maxPoint);
}

bool Disk::Intersect(const Ray &ray, float tMin, float& tMax,
	IntersectionResult &intersectionResult) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();

	float rayDotNormal = rayDirection * normalVec;
	
	if (fabs(rayDotNormal) <= EPSILON) {
		return false;
	}
	
	float t = (center - rayOrigin) * normalVec / rayDotNormal;
	
	if (t < tMin || t > tMax) {
		return false;
	}
	
	Point3 pointOnPlane = ray.GetPositionAtParam(t);
	
	if (center.GetDistanceSquared(pointOnPlane) < radiusSquared) {
		tMax = t;
		intersectionResult.SetIntersectionT(tMax);
		return true;
	}
	
	return false;
}

bool Disk::IntersectShadow(const Ray &ray, float tMin, float tMax) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();

	float rayDotNormal = rayDirection * normalVec;
	
	if (fabs(rayDotNormal) <= EPSILON) {
		return false;
	}
	
	float t = (center - rayOrigin) * normalVec / rayDotNormal;
	
	if (t < tMin || t > tMax) {
		return false;
	}
	
	Point3 pointOnPlane = ray.GetPositionAtParam(t);
	
	if (center.GetDistanceSquared(pointOnPlane) < radiusSquared) {
		return true;
	}
	
	return false;
}

Vector3 Disk::GetNormal(ParamsForNormal const &paramsForNormal) const {
	return normalVec;
}

Vector3 Disk::GetNormalAtPosition(Point3 const &position) const {
	return normalVec;
}

void Disk::SamplePrimitive(Point3& resultingSample) {
	// Leave out for now
}

float Disk::PDF(ParamsForNormal const &paramsForNormal) const {
	return 1.0f; // Doesn't return a valid value because we don't use it for sampling
}

AABBox Disk::GetBoundingBox() const {
	return boundingBoxLocal;
}
