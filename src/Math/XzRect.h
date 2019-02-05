#pragma once

#include "Math/Hittable.h"

class XzRect : public Hittable {
public:
	XzRect() { }
	XzRect(float _x0, float _x1, float _z0, float _z1,
		float _k, std::shared_ptr<Material> mat) :
		x0(_x0), x1(_x1), z0(_z0), z1(_z1),
		k(_k) { material = mat; };
	virtual bool Hit(const Ray& r, float tMin, float tMax,
		HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB &box) const {
		box = AABB(Vec3(x0, k-0.0001f, z0),
			Vec3(x1, k+0.0001f, z1));
		return true;
	}
;
	float x0, x1, z0, z1, k;
};

bool XzRect::Hit(const Ray& r, float tMin, float tMax,
		HitRecord& rec) const {
	auto rayOrigin = r.origin();
	auto rayDirection = r.direction();

	float t = (k - rayOrigin.y()) / rayDirection.y();
	if (t < tMin || t > tMax) {
		return false;
	}

	float x = rayOrigin.x() + t*rayDirection.x();
	float z = rayOrigin.z() + t*rayDirection.z();
	if (x < x0 || x > x1 || z < z0 || z > z1) {
		return false;
	}
	rec.u = (x-x0)/(x1-x0);
	rec.v = (z-z0)/(z1-z0);
	rec.t = t;
	rec.matPtr = material;
	rec.p = r.PointAtParam(t);
	rec.normal = Vec3(0.0, 1.0, 0.0);
	return true;
}
