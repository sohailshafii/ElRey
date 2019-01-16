#pragma once

#include "Math/Hittable.h"
#include "Math/Ray.h"

class Material {
public:
	Material() { }
	virtual ~Material() { }

	virtual bool scatter(const Ray &rIn, const HitRecord& rec,
		Vec3& attenuation, Ray& scattered) const = 0;
};
