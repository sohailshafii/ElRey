#pragma once

#include "Vec3.h"

class Ray {
public:
	Ray() {}
	Ray(const Vec3& a, const Vec3 &b, float ti = 0.0) { 
		A = a; B = b; _time = ti;
	}
	Vec3 origin() const { return A; }
	Vec3 direction() const { return B; }
	float time() const { return _time; }
	Vec3 PointAtParam(float t) const { return A + t*B; }

private:
	Vec3 A;
	Vec3 B;
	float _time;
};
