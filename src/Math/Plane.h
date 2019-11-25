#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"

class Plane : public Primitive {
public:
	Plane(const Point3& iOrigin, const Vector3& iNormal,
		const Color& iColor) : origin(iOrigin),
		normal(iNormal), color(iColor) {
		this->distance = -origin*normal;
	}

	bool Intersect(const Ray &ray, Color &newColor,
		float tMin, float& tMax) override;

private:
	Point3 origin;
	Vector3 normal;
	float distance;
	Color color;
};