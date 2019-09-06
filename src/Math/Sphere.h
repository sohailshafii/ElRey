#pragma once

#include "Primitive.h"
#include "Math/Point4.h"

class Sphere : public Primitive {
	Sphere(const Point4& iCenter, float iRadius) : center(iCenter),
		radius(iRadius) {
		radius = iRadius*iRadius;
	}

	bool Intersect(const Ray &ray, Color &newColor,
		float tMin, float& tMax) override;

private:
	Point4 center;
	float radius, radiusSqr;
};
