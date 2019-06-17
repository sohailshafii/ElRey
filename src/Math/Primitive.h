#pragma once

#include "Materials/Color.h"
#include "Math/Ray.h"

class Primitive {
public:
	Primitive() { }
	~Primitive() { }

	virtual bool GetHit(const Ray &ray, Color &newColor) = 0;

};
