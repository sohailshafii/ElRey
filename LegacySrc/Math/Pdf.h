#pragma once

#include "Vec3.h"

class Pdf {
public:
	virtual float value(const Vec3& direction) const = 0;
	virtual Vec3 generate() const = 0;
};
