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
		Vec3& attenuation, Ray& scattered, float &pdf) const = 0;
	virtual float scatteringPdf(const Ray &rIn, const HitRecord &rec,
		const Ray &scattered) const {
		return false;
	}
	virtual Vec3 emitted(float u, float v, const Vec3 &p) const {
		return Vec3(0.0, 0.0, 0.0);
	}

	std::shared_ptr<Texture> albedo;
};
