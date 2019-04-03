#pragma once

#include "Materials/Material.h"
#include "Materials/Texture.h"

class DiffuseLight : public Material {
public:
	DiffuseLight(std::shared_ptr<Texture> a) { 
		albedo = a;
	}

	virtual bool scatter(const Ray &rIn, const HitRecord& rec,
		Vec3& attenuation, Ray& scattered, float& pdf) const {
		pdf = 1.0f;
		return false;
	}

	virtual Vec3 emitted(float u, float v, const Vec3 &p) const {
		return albedo->Value(u, v, p);
	}

};
