#pragma once

#include "Material.h"
#include "Color.h"
#include "Materials/BRDFs/LambertianBRDF.h"

class LambertianMaterial : public Material {
public:
	LambertianMaterial(float ka, float kd, const Color& color);

	virtual Color GetAmbientColor(const IntersectionResult &intersectionResult)
		override;
	virtual Color GetDirectColor(const IntersectionResult &intersectionResult)
		override;
private:
	LambertianBRDF ambientBRDF;
	LambertianBRDF diffuseBRDF;
};
