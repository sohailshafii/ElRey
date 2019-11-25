#pragma once

#include "Primitive.h"
#include "Math/Point4.h"
#include <iostream>

class Sphere : public Primitive {
public:
	Sphere(const Point3& iCenter, float iRadius,
		const Color& iColor) : center(iCenter),
		color(iColor), radius(iRadius) {
		radiusSqr = radius*radius;
	}

	bool Intersect(const Ray &ray, Color &newColor,
		float tMin, float& tMax) override;

private:
	Point3 center;
	Color color;
	float radius, radiusSqr;
};
