#pragma once

#include "Materials/Material.h"

class Isotropic : public Material {
public:
	Isotropic(std::shared_ptr<Texture> a) { 
		albedo = a;
	}
	~Isotropic() {
	}

	virtual bool scatter(const Ray &rIn, const HitRecord& rec,
		Vec3& attenuation, Ray& scattered, float& pdf) const {
		scattered = Ray(rec.p, RandomPointInUnitSphere());
		attenuation = albedo->Value(rec.u, rec.v, rec.p);
		pdf = 1.0f;
		return true;
	}
};

