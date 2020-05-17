#pragma once

#include "Material.h"
#include "Color.h"
#include "Materials/BRDFs/LambertianBRDF.h"

class LambertianMaterial : public Material {
public:
	// TODO: mismatch between COlor4 and Color3 -- should we deal with transparency in BRDFs?
	LambertianMaterial(float ka, float kd, const Color3& color);

	virtual Color GetAmbientColor(const IntersectionResult &intersectionResult)
		override;
	virtual Color GetDirectColor(const IntersectionResult &intersectionResult)
		override;

	virtual Color GetColorForAreaLight(const IntersectionResult& intersectionResult)
		override;
private:
	LambertianBRDF ambientBRDF;
	LambertianBRDF diffuseBRDF;

	Color deadColor;
};
