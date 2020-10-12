#include "Triangle.h"
#include "Math/CommonMath.h"
#include "SceneData/IntersectionResult.h"
#include <cmath>
#include <algorithm>

bool Triangle::Intersect(const Ray &ray, float tMin, float& tMax,
						 IntersectionResult &intersectionResult) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	
	float a = p0[0] - p1[0], b = p0[0] - p2[0],
		c = rayDirection[0], d = p0[0] - rayOrigin[0];
	float e = p0[1] - p1[1], f = p0[1] - p2[1],
		g = rayDirection[1], h = p0[1] - rayOrigin[1];
	float i = p0[2] - p1[2], j = p0[2] - p2[2],
		k = rayDirection[2], l = p0[2] - rayOrigin[2];
	
	float m = f * k - g * j, n = h * k - g * l,
		p = f * l - h * j;
	float q = g * i - e * k, s = e * j - f * i;
	
	float invDenom = 1.0f / (a * m + b * q + c * s);
	
	float e1 = d * m - b * n - c * p;
	float beta = e1 * invDenom;
	
	if (beta < 0.0f) {
		return false;
	}
	
	float r = e * l - h * i;
	float e2 = a * n + d * q + c * r;
	float gamma = e2 * invDenom;
	
	if (gamma < 0.0f) {
		return false;
	}
	
	if ((beta + gamma) > 1.0f) {
		return false;
	}
	
	float e3 = a * p - b * r + d * s;
	float t = e3 * invDenom;
	
	if (t < tMin || t > tMax) {
		return false;
	}
	
	tMax = t;
	intersectionResult.SetIntersectionT(tMax);
	
	return true;
}

bool Triangle::IntersectShadow(const Ray &ray, float tMin, float tMax) {
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	
	float a = p0[0] - p1[0], b = p0[0] - p2[0],
		c = rayDirection[0], d = p0[0] - rayOrigin[0];
	float e = p0[1] - p1[1], f = p0[1] - p2[1],
		g = rayDirection[1], h = p0[1] - rayOrigin[1];
	float i = p0[2] - p1[2], j = p0[2] - p2[2],
		k = rayDirection[2], l = p0[2] - rayOrigin[2];
	
	float m = f * k - g * j, n = h * k - g * l,
		p = f * l - h * j;
	float q = g * i - e * k, s = e * j - f * i;
	
	float invDenom = 1.0f / (a * m + b * q + c * s);
	
	float e1 = d * m - b * n - c * p;
	float beta = e1 * invDenom;
	
	if (beta < 0.0f) {
		return false;
	}
	
	float r = e * l - h * i;
	float e2 = a * n + d * q + c * r;
	float gamma = e2 * invDenom;
	
	if (gamma < 0.0f) {
		return false;
	}
	
	if ((beta + gamma) > 1.0f) {
		return false;
	}
	
	float e3 = a * p - b * r + d * s;
	float t = e3 * invDenom;
	
	if (t < tMin || t > tMax) {
		return false;
	}
	
	return true;
}

void Triangle::SamplePrimitive(Point3& resultingSample) {
	// Not valid; necessary for sampling if we want area lights that are spheres
}

float Triangle::PDF(ParamsForNormal const &paramsForNormal) const {
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
	
	// in case any dimension is too thin, add some thickness
	if (fabs(maxPoint[2] - minPoint[2]) < EPSILON) {
		maxPoint[2] += 0.1f;
	}
	if (fabs(maxPoint[1] - minPoint[1]) < EPSILON) {
		maxPoint[1] += 0.1f;
	}
	if (fabs(maxPoint[0] - minPoint[0]) < EPSILON) {
		maxPoint[0] += 0.1f;
	}
	
	return AABBox(minPoint, maxPoint);
}

AABBox Triangle::GetBoundingBox() const {
	return boundingBox;
}
