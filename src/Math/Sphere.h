#pragma once

#include <cmath>
#include "Hittable.h"
#include "Materials/Material.h"
#include "Common.h"

class Sphere : public Hittable {
public:
	Sphere() {}
	Sphere(Vec3 cen, float r, Material *mat) : 
		center(cen), radius(r) { 
		material = mat;
	}

	virtual ~Sphere() { }

	virtual bool BoundingBox(float t0, float t1, AABB &box) const;
	virtual bool Hit(const Ray& r, float tMin, float tMax,
		HitRecord& rec) const;

	Vec3 center;
	float radius;
};

bool Sphere::BoundingBox(float t0, float t1, AABB &box) const {
	box = AABB(center - Vec3(radius, radius, radius),
		center + Vec3(radius, radius, radius));
	return true;
}

bool Sphere::Hit(const Ray& r, float tMin, float tMax,
	HitRecord& rec) const {
	// note that we had 2.0 in front of b and 4.0 in
	// front of a*c originally -- cancelled that with 2.0 in
	// denominator
	Vec3 centerToOrigin = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(centerToOrigin, r.direction());
	float c = dot(centerToOrigin, centerToOrigin) - radius*radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant))/a;
		rec.matPtr = material;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.PointAtParam(rec.t);
			Vec3 vecFromCenter = (rec.p-center)/radius;
			GetSphereUV(vecFromCenter, rec.u, rec.v);
			rec.normal = vecFromCenter;
			return true;
		}
		temp = (-b + sqrt(b*b - a*c))/a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.PointAtParam(rec.t);
			Vec3 vecFromCenter = (rec.p-center)/radius;
			GetSphereUV(vecFromCenter, rec.u, rec.v);
			rec.normal = vecFromCenter;
			return true;
		}
	}
	return false;
}
