#pragma once

#include "Vec3.h"

class Ray {
public:
	Ray() {}
	Ray(const Vec3& a, const Vec3 &b) { A = a; B = b; }
	Vec3 origin() const { return A; }
	Vec3 direction() const { return B; }
	Vec3 PointAtParam(float t) const { return A + t*B; }

	Vec3 A;
	Vec3 B;
};
