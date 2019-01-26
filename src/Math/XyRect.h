#pragma once

#include "Math/Hittable.h"

class XyRect : public Hittable {
public:
	XyRect() { }
	XyRect(float _x0, float _x1, float _y0, float _y1,
		float _k, Material *mat) :
		x0(_x1), x1(_x1), y0(_y0), y1(_y1),
		k(_k) { this->material = mat; };
	virtual bool Hit(const Ray& r, float tMin, float tMax,
		HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB &box) const {
		box = AABB(Vec3(x0, y0, k-0.0001),
			Vec3(x1, y1, k+0.0001));
		return true;
	}
;
	float x0, x1, y0, y1, k;
};

bool XyRect::Hit(const Ray& r, float tMin, float tMax,
		HitRecord& rec) const {
	auto rayOrigin = r.origin();
	auto rayDirection = r.direction();

	float t = (k - rayOrigin.z()) / rayDirection.z();
	if (t < tMin || t > tMax) {
		return false;
	}

	float x = rayOrigin.x() + t*rayDirection.x();
	float y = rayOrigin.y() + t*rayDirection.y();
	if (x < x0 || x > x1 || y < y0 || y > y1) {
		return false;
	}
	rec.u = (x-x0)/(x1-x0);
	rec.v = (y-y0)/(y1-y0);
	rec.t = t;
	rec.matPtr = this->material;
	rec.p = r.PointAtParam(t);
	rec.normal = Vec3(0.0, 0.0, 1.0);
	return false;
}
