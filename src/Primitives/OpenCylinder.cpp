#include "OpenCylinder.h"
#include "Math/CommonMath.h"
#include "SceneData/IntersectionResult.h"
#include <cmath>
#include <algorithm>
#include <cstdlib>

void OpenCylinder::GenerateBoundingBox() {
	boundingBox = AABBox(-radius, y0, -radius,
						 radius, y1, radius);
}

Primitive* OpenCylinder::Intersect(const Ray &ray, float tMin, float& tMax,
								   IntersectionResult &intersectionResult) {
	if (!boundingBox.RayHit(ray)) {
		return nullptr;
	}
	
	if (TestRayAndSetTMax(ray, tMin, tMax)) {
		intersectionResult.rayIntersectT = tMax;
		return this;
	}
	
	return nullptr;
}

bool OpenCylinder::IntersectShadow(const Ray &ray, float tMin, float tMax) {
	if (ignoreShadowTest) {
		return false;
	}
	return boundingBox.RayHit(ray) && TestRayAndSetTMax(ray, tMin, tMax) ?
		true : false;
}

Vector3 OpenCylinder::GetNormal(ShadingInfo const & shadingInfo) const {
	Vector3 rayDir = shadingInfo.eyeDir;
	Vector3 normalVec = ComputeHardNormal(shadingInfo.intersectionPosition);
	// inside surface?
	if (-rayDir*normalVec < 0.0f) {
		normalVec = -normalVec;
	}
	return normalVec;
}

Vector3 OpenCylinder::ComputeHardNormal(Point3 const &position) const {
	Vector3 normalVec = Vector3(position[0]*invRadius, 0.0f,
								position[2]*invRadius);
	normalVec.Normalize();
	return normalVec;
}

bool OpenCylinder::TestRayAndSetTMax(const Ray &ray, float tMin, float& tMax) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	
	float originX = rayOrigin[0];
	float originY = rayOrigin[1];
	float originZ = rayOrigin[2];
	float dirX = rayDirection[0];
	float dirY = rayDirection[1];
	float dirZ = rayDirection[2];

	float a = dirX*dirX + dirZ*dirZ;
	float b = 2.0f * (originX*dirX + originZ*dirZ);
	float c = originX*originX + originZ*originZ
		- radiusSqr;
	float discriminant =b * b - 4.0f * a * c;
	
	if (discriminant < 0.0f) {
		return false;
	}
	
	float discSqrt = sqrt(discriminant);
	float denominator = 2.0f * a;
	float t0 = (-b - discSqrt)/denominator;
	
	if (TestIfTMaxPasses(originY, dirY, t0, tMin, tMax)) {
		tMax = t0;
		return true;
	}
	
	float t1 = (-b + discSqrt)/denominator;
	if (TestIfTMaxPasses(originY, dirY, t1, tMin, tMax)) {
		tMax = t1;
		return true;
	}
	
	return false;
}

void OpenCylinder::SamplePrimitive(Point3& resultingSample,
								   const ShadingInfo& shadingInfo) {
	// Leave out for now
}

float OpenCylinder::PDF(const ShadingInfo& shadingInfo) const {
	return 1.0f; // Doesn't return a valid value because we don't use it for sampling
}

AABBox OpenCylinder::GetBoundingBox() const {
	return boundingBox;
}
