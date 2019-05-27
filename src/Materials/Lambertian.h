#pragma once

#include "Material.h"
#include "Math/Common.h"
#include "Materials/ConstantTexture.h"
#include "Math/Onb.h"

class Lambertian : public Material {
public:
	Lambertian(std::shared_ptr<Texture> a) { 
		albedo = a;
	}
	~Lambertian() {
	}

	virtual bool scatter(const Ray &rIn, const HitRecord& rec,
		Vec3& attenuation, Ray& scattered, float &pdf) const {
		//Vec3 target = rec.p + rec.normal + RandomPointInUnitSphere();
		ONB uvw;
		uvw.buildFromW(rec.normal);
		Vec3 direction;
		do {
			direction = uvw.local(RandomCosineDirection());
			//RandomPointInUnitSphere();
		} while(dot(direction, rec.normal) < 0);

		scattered = Ray(rec.p, unitVector(direction), rIn.time());
		//Ray(rec.p, target - rec.p, rIn.time());
		attenuation = albedo->Value(rec.u, rec.v, rec.p);
		pdf = dot(rec.normal, scattered.direction())/(float)M_PI;
		return true;
	}

	virtual float scatteringPdf(const Ray& rIn, const HitRecord &rec,
		const Ray &scattered) const {
		float cosine = dot(rec.normal, unitVector(scattered.direction()));
		if (cosine < 0) cosine = 0;
		return cosine / (float)M_PI;
	}
};
