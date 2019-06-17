#pragma once

#include "Materials/Color.h"
#include "Math/Ray.h"

class Primitive {
public:
	Primitive() { }
	~Primitive() { }

	virtual bool Intersect(const Ray &ray, Color &newColor,
		float& tMin, float tMax) = 0;

};
