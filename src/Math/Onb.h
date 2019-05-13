#pragma once

#include <cmath>
#include "Vec3.h"

class ONB {
public:
	ONB() { }
	inline Vec3 operator[](int i) const { return axis[i]; }
	Vec3 u() const { return axis[0]; }
	Vec3 v() const { return axis[1]; }
	Vec3 w() const { return axis[2]; }

	Vec3 local(float a, float b, float c) const {
		return a*u() + b*v() + c*w();
	}
	Vec3 local(const Vec3& a) {
		return a.x()*u() + a.y()*v() + a.z()*w();
	}

	void buildFromW(const Vec3&);

	Vec3 axis[3];
};

void ONB::buildFromW(const Vec3& n) {
	axis[2] = unitVector(n);
	Vec3 a;
	if (fabs(w().x()) > 0.9) {
		a = Vec3(0, 1, 0);
	}
	else {
		a = Vec3(1, 0, 0);
	}
	axis[1] = unitVector(cross(w(), a));
	axis[0] = cross(w(), v());
}
