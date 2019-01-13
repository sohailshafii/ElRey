#pragma once

#include "Vec3.h"

// create random points in unit cube (-1.0 to 1.0) and reject if not
// in sphere
Vec3 RandomPointInUnitSphere() {
	Vec3 p;
	do {
		p = 2.0*Vec3(drand48(),drand48(),drand48()) - Vec3(1,1,1);
	} while(p.squaredLength() >= 1.0);
	return p;
}

bool Refract(const Vec3& v, const Vec3& n, float niOverNt, Vec3 &refracted) {
	Vec3 uv = unitVector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - niOverNt*niOverNt*(1-dt*dt);
	if (discriminant > 0) {
		refracted = niOverNt*(uv - n*dt) - n*sqrt(discriminant);
		return true;
	}
	else {
		return false;
	}
}
