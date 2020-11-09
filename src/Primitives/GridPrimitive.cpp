#include "GridPrimitive.h"

bool GridPrimitive::Intersect(const Ray &ray, float tMin, float& tMax,
			   IntersectionResult &intersectionResult) {
	return false; // TODO
}

bool GridPrimitive::IntersectShadow(const Ray &ray, float tMin, float tMax) {
	return false; // TODO
}

Vector3 GridPrimitive::GetNormal(ParamsForNormal const &paramsForNormal) const {
	return Vector3(); // TODO
}

// useful if you just want to query a normal without intersection information
Vector3 GridPrimitive::GetNormalAtPosition(Point3 const &position) const {
	return Vector3(); // TODO
}

void GridPrimitive::SamplePrimitive(Point3& resultingSample) {
	// TODO
}


