#pragma once

#include "Math/Hittable.h"

class YzRect : public Hittable {
public:
	YzRect() { }
	YzRect(float _y0, float _y1, float _z0, float _z1,
		float _k, std::shared_ptr<Material> mat) :
		y0(_y0), y1(_y1), z0(_z0), z1(_z1),
		k(_k) { material = mat; };
	virtual bool Hit(const Ray& r, float tMin, float tMax,
		HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB &box) const {
		box = AABB(Vec3(k-0.0001, y0, z0),
			Vec3(k+0.0001, y1, z1));
		return true;
	}
;
	float y0, y1, z0, z1, k;
};

bool YzRect::Hit(const Ray& r, float tMin, float tMax,
		HitRecord& rec) const {
	auto rayOrigin = r.origin();
	auto rayDirection = r.direction();

	float t = (k - rayOrigin.x()) / rayDirection.x();
	if (t < tMin || t > tMax) {
		return false;
	}

	float y = rayOrigin.y() + t*rayDirection.y();
	float z = rayOrigin.z() + t*rayDirection.z();
	if (y < y0 || y > y1 || z < z0 || z > z1) {
		return false;
	}
	rec.u = (y-y0)/(y1-y0);
	rec.v = (z-z0)/(z1-z0);
	rec.t = t;
	rec.matPtr = material;
	rec.p = r.PointAtParam(t);
	rec.normal = Vec3(1.0, 0.0, 0.0);
	return true;
}
