#pragma once

#include "Material.h"
#include "Math/Common.h"
#include "Materials/ConstantTexture.h"

class Lambertian : public Material {
public:
	Lambertian(std::shared_ptr<Texture> a) { 
		albedo = a;
	}
	~Lambertian() {
	}

	virtual bool scatter(const Ray &rIn, const HitRecord& rec,
		Vec3& attenuation, Ray& scattered) const {
		Vec3 target = rec.p + rec.normal + RandomPointInUnitSphere();
		scattered = Ray(rec.p, target - rec.p, rIn.time());
		attenuation = albedo->Value(rec.u, rec.v, rec.p);
		return true;
	}
};
