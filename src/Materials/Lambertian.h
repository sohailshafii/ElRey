#pragma once

#include "Material.h"
#include "Math/Common.h"
#include "Materials/ConstantTexture.h"

class Lambertian : public Material {
public:
	Lambertian(Texture* a) : albedo(a) { }
	~Lambertian() {
		if (albedo != nullptr) {
			delete albedo;
		}
	}

	virtual bool scatter(const Ray &rIn, const HitRecord& rec,
		Vec3& attenuation, Ray& scattered) const {
		Vec3 target = rec.p + rec.normal + RandomPointInUnitSphere();
		scattered = Ray(rec.p, target - rec.p, rIn.time());
		attenuation = albedo->Value(0, 0, rec.p);
		return true;
	}

	Texture *albedo;
};
