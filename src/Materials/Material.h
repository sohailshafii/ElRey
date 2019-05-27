#pragma once

#include <memory>
#include "Math/Hittable.h"
#include "Math/Ray.h"

#include "Materials/Texture.h"
#include "Math/Pdf.h"

struct ScatterRecord {
	Ray specularRay;
	bool isSpecular;
	Vec3 attenuation;
	std::shared_ptr<Pdf> pdfPtr;
};

class Material {
public:
	Material() { albedo = nullptr; }
	virtual ~Material() { }

	virtual bool scatter(const Ray &rIn, const HitRecord& rec,
		ScatterRecord &srec) const {
		return false;
	}
	virtual float scatteringPdf(const Ray &rIn, const HitRecord &rec,
		const Ray &scattered) const {
		return false;
	}
	virtual Vec3 emitted(const Ray& rIn,
		const HitRecord& rec,
		float u, float v, const Vec3 &p) const {
		return Vec3(0.0, 0.0, 0.0);
	}

	std::shared_ptr<Texture> albedo;
};
