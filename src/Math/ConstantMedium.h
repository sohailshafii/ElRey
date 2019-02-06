#pragma once

#include <iostream>
#include <limits>
#include "Math/Common.h"
#include "Materials/Isotropic.h"
#include "Math/Hittable.h"

class ConstantMedium : public Hittable {
public:
	ConstantMedium(Hittable* b, float d,
		std::shared_ptr<Texture> albedo) {
		boundary = b;
		density = d;
		material = std::make_shared<Isotropic>(Isotropic(albedo));
	}

	virtual bool Hit(const Ray &r, float tMin, float tMax,
		HitRecord &rec) const ;
	virtual bool BoundingBox(float t0, float t1,
		AABB &box) const;

	Hittable* boundary;
	float density;
};

bool ConstantMedium::Hit(const Ray &r, float tMin, float tMax,
	HitRecord &rec) const {
	bool db = getRand() < 0.00001f;
	db = false;
	HitRecord rec1, rec2;
	if (boundary->Hit(r, -std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max(), rec1) &&
		boundary->Hit(r, rec1.t+0.0001f,
			std::numeric_limits<float>::max(), rec2)) {
		if (db) {
			std::cerr << "\nt0 t1 " << rec1.t <<
				" " << rec2.t << "\n";
		}
		if (rec1.t < tMin) {
			rec1.t = tMin;
		}
		if (rec2.t > tMax) {
			rec2.t = tMax;
		}
		if (rec1.t >= rec2.t) {
			return false;
		}
		if (rec1.t < 0.0) {
			rec1.t = 0.0;
		}

		float distanceInsideBoundary = (rec2.t - rec1.t)*
			r.direction().length();
		float hitDistance = -(1.0f / density)*log(getRand());
		if (hitDistance < distanceInsideBoundary) {
			if (db) std::cerr << "hit distance: " << hitDistance << "\n";
			rec.t = rec1.t + hitDistance / r.direction().length();
			if (db) std::cerr << "rec.t = " << rec.t << "\n";
			rec.p = r.PointAtParam(rec.t);
			if (db) std::cerr << "rec.p = " << rec.p[0] << " " << rec.p[1] << " " << rec.p[2] << "\n";
			rec.normal = Vec3(1.0, 0.0, 0.0); // arbitrary
			rec.matPtr = material;
			return true;
		}
	}
	return false;
}

bool ConstantMedium::BoundingBox(float t0, float t1,
	AABB &box) const {
	return boundary->BoundingBox(t0, t1, box);
}