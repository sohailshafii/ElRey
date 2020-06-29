#include "CompoundObject.h"

bool CompoundObject::Intersect(const Ray &ray, float tMin, float& tMax,
			   IntersectionResult &intersectionResult) {
	// TODO
	return false;
}

bool CompoundObject::IntersectShadow(const Ray &ray, float tMin,
									 float tMax) {
	// TODO
	return false;
}

void CompoundObject::SamplePrimitive(Point3& resultingSample) {
	// nothing to see here
}

float CompoundObject::PDF(const IntersectionResult& intersectionResult) const {
	return 0.0f;
	// nothing to see here
}
