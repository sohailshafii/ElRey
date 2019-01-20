#pragma once

#include "Hittable.h"
#include "Math/Vec3.h"
#include "Math/Ray.h"
#include "Math/AABB.h"
#include "Materials/Material.h"

class MovingSphere : public Hittable {
public:
	MovingSphere() {
	}
	MovingSphere(Vec3 cen0, Vec3 cen1, float t0, float t1,
		float r, Material *m) :
		center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r)
		{ material = m; }

	virtual bool Hit(const Ray& r, float tmin, float tmax,
		HitRecord &rec) const;
	virtual bool BoundingBox(float t0, float t1,
		AABB &box) const;

	Vec3 Center(float time) const;

	Vec3 center0, center1;
	float time0, time1;
	float radius;
};

Vec3 MovingSphere::Center(float time) const {
	return center0 + ((time - time0)/(time1 - time0))*
		(center1 - center0);
}

bool MovingSphere::Hit(const Ray& r, float tMin, float tMax,
	HitRecord& rec) const {
	auto timedCenter = Center(r.time());

	// note that we had 2.0 in front of b and 4.0 in
	// front of a*c originally -- cancelled that with 2.0 in
	// denominator
	Vec3 centerToOrigin = r.origin() - timedCenter;
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
			rec.normal = (rec.p - timedCenter)/radius;
			return true;
		}
		temp = (-b + sqrt(b*b - a*c))/a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.PointAtParam(rec.t);
			rec.normal = (rec.p - timedCenter)/radius;
			return true;
		}
	}
	return false;
}

bool MovingSphere::BoundingBox(float t0, float t1,
		AABB &box) const {
	Vec3 radiusVec = Vec3(radius, radius, radius);
	Vec3 Center0 = Center(t0), Center1 = Center(t1);

	AABB box0(Center0 - radiusVec, 
		Center0 + radiusVec);
	AABB box1(Center1 - radiusVec, 
		Center1 + radiusVec);
	box = SurroundingBox(box0, box1);
	return true;
}
