#pragma once

#include "Math/Hittable.h"

class Translate : public Hittable {
public:
	Translate(Hittable *p, const Vec3& displacement)
		: ptr(p), offset(displacement) { }
	~Translate() {
		if (ptr != nullptr) {
			delete ptr;
		}
	}

	virtual bool Hit(const Ray &r, float tMin, float tMax,
		HitRecord &rec) const;
	virtual bool BoundingBox(float t0, float t1,
		AABB &box) const;

	Hittable *ptr;
	Vec3 offset;
};

bool Translate::Hit(const Ray &r, float tMin, float tMax,
	HitRecord &rec) const {

	// the equivalent to moving an object is to
	// move ray in opposite direction. then adjust
	// the actual intersected position using same
	// offset, except in correct direction.
	Ray movedR(r.origin() - offset, r.direction(),
		r.time());
	if (ptr->Hit(movedR, tMin, tMax, rec)) {
		rec.p += offset;
		return true;
	}
	else {
		return false;
	}
}

bool Translate::BoundingBox(float t0, float t1,
	AABB &box) const {
	if (ptr->BoundingBox(t0, t1, box)) {
		box = AABB(box.Min() + offset,
			box.Max() + offset);
		return true;
	}
	else {
		return false;
	}
}
