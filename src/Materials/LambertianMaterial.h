#pragma once

#include "Material.h"
#include "Color.h"
#include "Materials/BRDFs/LambertianBRDF.h"

class LambertianMaterial : public Material {
public:
	LambertianMaterial(float ka, float kd, const Color& c);

	// TODO: include lights array here or at least the scene
	virtual Color GetColor(const IntersectionResult &intersectionResult)
		override;

private:
	LambertianBRDF ambientBRDF;
	LambertianBRDF diffuseBRDF;
};
