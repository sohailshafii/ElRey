#pragma once

#include "Math/Vec3.h"

class Texture {
public:
	virtual Vec3 Value(float u, float v, const Vec3 &p) const = 0;

	virtual ~Texture() { }
};
