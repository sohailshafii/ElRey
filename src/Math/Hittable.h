#pragma once

#include "Ray.h"
#include "AABB.h"

#include <memory>

class Material;

struct HitRecord {
	float t;
	float u, v;
	Vec3 p;
	Vec3 normal;
	std::shared_ptr<Material> matPtr;
};

class Hittable {
public:
	Hittable() { material = nullptr; }

	virtual bool Hit(const Ray &r, float tMin, float tMax,
		HitRecord &rec) const = 0;
	virtual bool BoundingBox(float t0, float t1,
		AABB &box) const = 0;

	virtual ~Hittable() { 
	}

	std::shared_ptr<Material> material;
};
