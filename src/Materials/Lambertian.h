#pragma once

#include "Material.h"
#include "Math/Common.h"

class Lambertian : public Material {
public:
	Lambertian(const Vec3& a) : albedo(a) { }
	virtual bool scatter(const Ray &rIn, const HitRecord& rec,
		Vec3& attenuation, Ray& scattered) const {
		Vec3 target = rec.p + rec.normal + RandomPointInUnitSphere();
		scattered = Ray(rec.p, target - rec.p, rIn.time());
		attenuation = albedo;
		return true;
	}

	Vec3 albedo;
};
