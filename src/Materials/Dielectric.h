#pragma once

#include "Material.h"

class Dielectric : public Material {
public:
	Dielectric(float ri) : refIdx(ri) { }

	virtual bool scatter(const Ray& rIn, const HitRecord &rec,
		Vec3 &attenuation, Ray& scattered) const {
		
		Vec3 outwardNormal;
		Vec3 reflectedVec = reflect(rIn.direction(), rec.normal);
		float niOverNt;
		// try playing with this atten
		attenuation = Vec3(1.0, 1.0, 1.0);
		Vec3 refractedVector;
		float reflectedProb, cosineTerm;

		auto rInDir = rIn.direction(); 

		// coming from outside of surface?
		if (dot(rInDir, rec.normal) > 0) {
			outwardNormal = -rec.normal;
			niOverNt = refIdx;
			// can divide by one item's length if normal is normalized
			cosineTerm = refIdx * dot(rIn.direction(), rec.normal)
				/ rIn.direction().length();
		}
		else {
			outwardNormal = rec.normal;
			niOverNt = 1.0f / refIdx;
			cosineTerm = -dot(rIn.direction(), rec.normal) /
				rIn.direction().length();
		}

		if (Refract(rInDir, outwardNormal, niOverNt, refractedVector)) {
			reflectedProb = Schlick(cosineTerm, refIdx);
		}
		else {
			reflectedProb = 1.0;
		}

		if (getRand() < reflectedProb) {
			scattered = Ray(rec.p, reflectedVec);
		}
		else {
			scattered = Ray(rec.p, refractedVector);
		}

		return true;
	}

	float refIdx;
};
