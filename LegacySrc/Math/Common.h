#pragma once

#include "Vec3.h"

Vec3 deNan(const Vec3& c) {
	Vec3 temp = c;
	if (!(temp[0] == temp[0])) temp[0] = 0.0f;
	if (!(temp[1] == temp[1])) temp[1] = 0.0f;
	if (!(temp[2] == temp[2])) temp[2] = 0.0f;
	return temp;
}

inline float getRand() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

inline Vec3 RandomCosineDirection() {
	float r1 = getRand();
	float r2 = getRand();
	float z = sqrt(1.0f - r2);
	float phi = 2.0f*(float)M_PI*r1;
	float x = cos(phi)*2.0f*sqrt(r2);
	float y = sin(phi)*2.0f*sqrt(r2);
	return Vec3(x, y, z);
}

// create random points in unit cube (-1.0 to 1.0) and reject if not
// in sphere
Vec3 RandomPointInUnitSphere() {
	Vec3 p;
	do {
		p = 2.0*Vec3(getRand(), getRand(), getRand()) - Vec3(1,1,1);
	} while(p.squaredLength() >= 1.0);
	return p;
}

Vec3 RandomPointOnUnitSphere() {
	Vec3 p;
	do {
		p = 2.0*Vec3(getRand(), getRand(), getRand()) - Vec3(1,1,1);
	} while(p.squaredLength() >= 1.0);
	p.makeUnitVector();
	return p;
}

Vec3 RandomToSphere(float radius, float distanceSquared) {
	float r1 = getRand();
	float r2 = getRand();
	float z = 1.0f + r2*(sqrt(1.0f - radius*radius / distanceSquared) - 1.0f);
	float phi = 2.0f*(float)M_PI*r1;
	float x = cos(phi)*sqrt(1.0f - z*z);
	float y = sin(phi)*sqrt(1.0f - z*z);
	return Vec3(x, y, z);
}

Vec3 RandomInUnitDisk() {
	Vec3 p;
	do {
		p = 2.0*Vec3(getRand(), getRand(), 0) - Vec3(1.0, 1.0, 0.0);
	} while (dot(p, p) >= 1.0);
	return p;
}

bool Refract(const Vec3& v, const Vec3& n, float niOverNt, Vec3 &refracted) {
	Vec3 uv = unitVector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0f - niOverNt*niOverNt*(1.0f-dt*dt);
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

// TODO: do this without a loop
inline float TrilinearInterp(float c[2][2][2], float u, float v, float w) {
	float accum = 0.0;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			for (int z = 0; z < 2; z++) {
				accum += (x*u + (1-x)*(1-u))*
						(y*v + (1-y)*(1-v))*
						(z*w + (1-z)*(1-w))*c[x][y][z];
			}
		}
	}
	return accum;
}

void GetSphereUV(const Vec3& p, float& u, float& v) {
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	u = 1.0f - (phi + (float)M_PI)/(2.0f*(float)M_PI);
	v = (theta + (float)M_PI*0.5f)/ (float)M_PI;
}

