#pragma once

#include "Primitive.h"
#include "Math/Point4.h"
#include "Math/Vector3.h"

class Plane : public Primitive {
public:
	Plane(const Point4& origin, const Vector3& normal,
		const Color& color) {
		this->origin = origin;
		this->normal = normal;
		this->color = color;
		//distance = -origin*normal;
	}

	bool Intersect(const Ray &ray, Color &newColor,
		float& tMin, float tMax) override;

private:
	Point4 origin;
	Vector3 normal;
	float distance;
	Color color;
};