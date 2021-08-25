#include "Triangle.h"
#include "Math/CommonMath.h"
#include "SceneData/IntersectionResult.h"
#include <cmath>
#include <algorithm>

Primitive* Triangle::Intersect(const Ray &ray, float tMin, float& tMax,
							   IntersectionResult &intersectionResult) {
	float t, beta, gamma;
	if (!CommonMath::TestTriangle(p0, p1, p2, ray, tMin, tMax, t,
								  beta, gamma)) {
		return nullptr;
	}
	
	tMax = t;
	intersectionResult.rayIntersectT = tMax;
	
	return this;
}

bool Triangle::IntersectShadow(const Ray &ray, float tMin, float tMax) {
	if (ignoreShadowTest) {
		return false;
	}
	
	float t, beta, gamma;
	return CommonMath::TestTriangle(p0, p1, p2, ray, tMin, tMax, t,
									beta, gamma);
}

void Triangle::SamplePrimitive(Point3& resultingSample,
							   const ShadingInfo &shadingInfo) {
	// Not valid; necessary for sampling if we want area lights that are spheres
}

float Triangle::PDF(const ShadingInfo &shadingInfo) const {
	return 1.0f; // invalid until we need to use it
}

AABBox Triangle::ComputeBoundingBox() const {
	Point3 minPoint = p0;
	Point3 maxPoint = p0;
	
	minPoint[0] = std::min(minPoint[0], p1[0]);
	minPoint[0] = std::min(minPoint[0], p2[0]);
	
	minPoint[1] = std::min(minPoint[1], p1[1]);
	minPoint[1] = std::min(minPoint[1], p2[1]);
	
	minPoint[2] = std::min(minPoint[2], p1[2]);
	minPoint[2] = std::min(minPoint[2], p2[2]);
	
	maxPoint[0] = std::max(maxPoint[0], p1[0]);
	maxPoint[0] = std::max(maxPoint[0], p2[0]);
	
	maxPoint[1] = std::max(maxPoint[1], p1[1]);
	maxPoint[1] = std::max(maxPoint[1], p2[1]);
	
	maxPoint[2] = std::max(maxPoint[2], p1[2]);
	maxPoint[2] = std::max(maxPoint[2], p2[2]);
	
	minPoint[0] -= 0.1f;
	minPoint[1] -= 0.1f;
	minPoint[2] -= 0.1f;
	maxPoint[0] += 0.1f;
	maxPoint[1] += 0.1f;
	maxPoint[2] += 0.1f;
	
	return AABBox(minPoint, maxPoint);
}

AABBox Triangle::GetBoundingBox() const {
	return boundingBox;
}
