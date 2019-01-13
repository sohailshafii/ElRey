#pragma once

// TODO: fix this stupid problem in cmake -- shouldn't have to do .. in path
#include "../Math/Hittable.h"
#include "../Math/Ray.h"

class Material {
public:
	Material() { }
	virtual ~Material() { }

	virtual bool scatter(const Ray &rIn, const HitRecord& rec,
		Vec3& attenuation, Ray& scattered) const = 0;
};
