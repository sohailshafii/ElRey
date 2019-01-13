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

Vec3 RandomInUnitDisk() {
	Vec3 p;
	do {
		p = 2.0*Vec3(drand48(), drand48(), 0) - Vec3(1.0, 1.0, 0.0);
	} while (dot(p, p) >= 1.0);
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

// looking at a window at a steep angle and it becomes a mirror
// Schlick approximation of Fresnel factor (objects become
// reflective toward edges)
float Schlick(float cosine, float refIdx) {
	float r0 = (1 - refIdx)/(1 + refIdx);
	r0 = r0*r0;
	return r0 + (1-r0)*pow((1-cosine), 5);
}

