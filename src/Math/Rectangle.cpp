#include "Rectangle.h"
#include "CommonMath.h"

bool Rectangle::Intersect(const Ray &ray, float tMin, float& tMax,
					  IntersectionResult &intersectionResult) {
	return false; // TODO
}

bool Rectangle::IntersectShadow(const Ray &ray, float tMin, float tMax)
{
	return false; // TODO
}

void Rectangle::SamplePrimitive(Point3& resultingSample) {
	// TODO
}

float Rectangle::PDF(IntersectionResult& intersectionResult) const {
	return 1.0f; // TODO
}

