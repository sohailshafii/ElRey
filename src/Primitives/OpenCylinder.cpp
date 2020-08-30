#include "OpenCylinder.h"
#include "Math/CommonMath.h"
#include "SceneData/IntersectionResult.h"
#include <cmath>
#include <algorithm>
#include <cstdlib>

void OpenCylinder::GenerateBoundingBox() {
	boundingBoxLocal = AABBox(-radius, y0, -radius,
							  radius, y1, radius);
}

bool OpenCylinder::IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
	IntersectionResult &intersectionResult) {
	if (!boundingBoxLocal.RayHit(rayLocal)) {
		return false;
	}
	
	if (TestRayAndSetTMax(rayLocal, tMin, tMax)) {
		intersectionResult.SetIntersectionT(tMax);
		return true;
	}
	
	return false;
}

bool OpenCylinder::IntersectShadowLocal(const Ray &rayLocal, float tMin, float tMax) {
	return boundingBoxLocal.RayHit(rayLocal) && TestRayAndSetTMax(rayLocal, tMin, tMax);
}

Vector3 OpenCylinder::GetNormalWorld(IntersectionResult const &intersectionResult) const {
	Ray const & incomingRay = intersectionResult.GetIncomingRay();
	Vector3 rayDir = incomingRay.GetDirection();
	Point3 intersectLocal = intersectionResult.GetIntersectionPosLocal();
	Vector3 normalVec = Vector3(intersectLocal[0]*invRadius, 0.0f,
								intersectLocal[2]*invRadius);
	
	// inside surface?
	if (-rayDir*normalVec < 0.0f) {
		normalVec = -normalVec;
	}
	normalVec.Normalize();
	return isTransformed ? GetWorldToLocalTransposeDir(normalVec).Normalized() : normalVec;
}

bool OpenCylinder::TestRayAndSetTMax(const Ray &rayLocal, float tMin, float& tMax) {
	const Point3& rayOrigin = rayLocal.GetOrigin();
	const Vector3& rayDirection = rayLocal.GetDirection();
	
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

void OpenCylinder::SamplePrimitiveLocal(Point3& resultingSample) {
	// Leave out for now
}

void OpenCylinder::SamplePrimitiveWorld(Point3& resultingSample) {
	// Leave out for now
}

float OpenCylinder::PDF(const IntersectionResult& intersectionResult) const {
	return 1.0f; // Doesn't return a valid value because we don't use it for sampling
}

AABBox OpenCylinder::GetBoundingBoxLocal() const {
	return boundingBoxLocal;
}

AABBox OpenCylinder::GetBoundingBoxWorld() const {
	if (!isTransformed) {
		return boundingBoxLocal;
	}
	auto boundingBoxMin = boundingBoxLocal.GetMin();
	auto boundingBoxMax = boundingBoxLocal.GetMax();
	return AABBox(GetLocalToWorldPos(boundingBoxMin),
				  GetLocalToWorldPos(boundingBoxMax));
}
