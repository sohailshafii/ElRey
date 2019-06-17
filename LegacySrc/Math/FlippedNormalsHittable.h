#pragma once

#include "Math/Hittable.h"

class FlippedNormalsHittable : public Hittable {
public:

	FlippedNormalsHittable(std::shared_ptr<Hittable> p) {
		ptr = p;
	}

	virtual bool Hit(const Ray &r, float tMin, float tMax,
		HitRecord &rec) const {
		if (ptr->Hit(r, tMin, tMax, rec)) {
			rec.normal = -rec.normal;
			return true;
		}
		else {
			return false;
		}
	}

	virtual bool BoundingBox(float t0, float t1,
		AABB &box) const {
		return ptr->BoundingBox(t0, t1, box);
	}

	std::shared_ptr<Hittable> ptr;
};
