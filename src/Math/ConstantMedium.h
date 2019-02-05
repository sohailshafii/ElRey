#pragma once

#include "Material/Isotropic.h"
#include "Math/Hittable.h"

class ConstantMedium : public Hittable {
public:
	ConstantMedium(std::shared_ptr<Hittable> b, float d,
		std::shared_ptr<Texture> albedo) {
		boundary = b;
		density = d;
		material = std::make_shared<Isotropic>(Isotropic(a));
	}

	virtual bool Hit(const Ray &r, float tMin, float tMax,
		HitRecord &rec) const ;
	virtual bool BoundingBox(float t0, float t1,
		AABB &box) const;

	std::shared_ptr<Hittable> boundary;
	float density;

};

bool ConstantMedium::Hit(const Ray &r, float tMin, float tMax,
	HitRecord &rec) const {

}

bool ConstantMedium::BoundingBox(float t0, float t1,
	AABB &box) const {

}