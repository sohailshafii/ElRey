#pragma once

#include "Material.h"
#include "Math/Common.h"

class Metal : public Material {
public:
	Metal(const Vec3 &a, float f = 0.0) : albedo(a) { 
		fuzz = (f < 1.0f) ? f : 1.0f;
	}

	virtual bool scatter(const Ray &rIn, const HitRecord& rec,
		ScatterRecord &sRec) const {
		/*Vec3 reflectedVec = reflect(unitVector(rIn.direction()),
			rec.normal);
		// add a bit of randomness to reflected Vec
		scattered = Ray(rec.p, reflectedVec
			+ fuzz*RandomPointInUnitSphere());
		attenuation = albedo;
		pdf = 1.0;
		return (dot(scattered.direction(), rec.normal) > 0);*/
		Vec3 reflectedVec = reflect(unitVector(rIn.direction()),
			rec.normal);
		sRec.specularRay = Ray(rec.p, reflectedVec
			+ fuzz*RandomPointInUnitSphere());
		sRec.attenuation = albedo;
		sRec.isSpecular = true;
		sRec.pdfPtr = nullptr;
		return true;
	}

	Vec3 albedo;
	float fuzz;
};
