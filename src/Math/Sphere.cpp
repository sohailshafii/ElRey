#include "Sphere.h"
#include "Math/Vector3.h"
#include "Math/CommonMath.h"
#include <cmath>
#include <iostream>

bool Sphere::Intersect(const Ray &ray, Color &newColor,
	float tMin, float& tMax) {
	const Point4& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();

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
		newColor = color;
		// normal is (temp + t * rayDirection)/radius;
		// local hit point is rayOrigin + t*rayDirection;
		return true;
	}

	t = (-b+e)/denom;
	if (t > EPSILON && t > tMin && t < tMax) {
		tMax = t;
		newColor = color;
		// normal is (temp + t * rayDirection)/radius;
		// local hit point is rayOrigin + t*rayDirection;
		return true;
	}

	// all tests failed so far
	return false;
}