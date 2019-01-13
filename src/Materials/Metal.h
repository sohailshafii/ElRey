#pragma once

#include "Material.h"
#include "../Math/Common.h"

class Metal : public Material {
public:
	Metal(const Vec3 &a, float f) : albedo(a) { 
		fuzz = (f < 1) ? f : 1.0;
	}

	virtual bool scatter(const Ray &rIn, const HitRecord& rec,
		Vec3& attenuation, Ray& scattered) const {
		Vec3 reflectedVec = reflect(unitVector(rIn.direction()),
			rec.normal);
		// add a bit of randomness to reflected Vec
		scattered = Ray(rec.p, reflectedVec
			+ fuzz*RandomPointInUnitSphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	Vec3 albedo;
	float fuzz;
};
