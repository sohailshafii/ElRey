#pragma once

#include "Ray.h"
#include "AABB.h"

class Material;

struct HitRecord {
	float t;
	Vec3 p;
	Vec3 normal;
	Material *matPtr;
};

class Hittable {
public:
	virtual bool Hit(const Ray &r, float tMin, float tMax,
		HitRecord &rec) const = 0;
	virtual bool BoundingBox(float t0, float t1,
		AABB &box) const = 0;

	virtual ~Hittable() { }

	Material *material;
};
