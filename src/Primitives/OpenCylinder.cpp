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

bool OpenCylinder::Intersect(const Ray &ray, float tMin, float& tMax,
	IntersectionResult &intersectionResult) {
	if (!boundingBox.RayHit(ray)) {
		return false;
	}
	
	if (TestRayAndSetTMax(ray, tMin, tMax)) {
		intersectionResult.SetIntersectionT(tMax);
		return true;
	}
	
	return false;
}

bool OpenCylinder::IntersectShadow(const Ray &ray, float tMin, float tMax) {
	return boundingBox.RayHit(ray) && TestRayAndSetTMax(ray, tMin, tMax);
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
	float b = 2.0f * (originX*originX + originZ*originZ);
	float c = originX*originX + originZ*originZ
		- radiusSqr;
	float discriminant = b * b - 4.0 * a * c;
	
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

void OpenCylinder::SamplePrimitive(Point3& resultingSample) {
	// Leave out for now
}

float OpenCylinder::PDF(const IntersectionResult& intersectionResult) const {
	return 1.0f; // Doesn't return a valid value because we don't use it for sampling
}

AABBox OpenCylinder::GetBoundingBox() const {
	return boundingBox;
}

