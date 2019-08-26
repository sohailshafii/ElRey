#include "Plane.h"

bool Plane::Intersect(const Ray &ray, Color &newColor,
	float& tMin, float tMax) {
	const Point4& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();

	float vDotNormal = rayDirection*normal;
	float t = -(rayOrigin*normal + distance)/vDotNormal;

	if (t < tMin || t > tMax) {
		return false;
	}
	t = tMin;

	newColor = color;
	return true;
}