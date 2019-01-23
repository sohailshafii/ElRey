#pragma once

#include <memory>
#include "Math/Hittable.h"
#include "Math/Ray.h"

#include "Materials/Texture.h"

class Material {
public:
	Material() { albedo = nullptr; }
	virtual ~Material() { }

	virtual bool scatter(const Ray &rIn, const HitRecord& rec,
		Vec3& attenuation, Ray& scattered) const = 0;

	std::shared_ptr<Texture> albedo;
};
