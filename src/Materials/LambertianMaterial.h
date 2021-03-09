#pragma once

#include "Material.h"
#include "Color.h"
#include "Materials/BRDFs/LambertianBRDF.h"

class LambertianMaterial : public Material {
public:
	// TODO: mismatch between Color4 and Color3 -- should we deal with transparency in BRDFs?
	LambertianMaterial(float ka, float kd, const Color3& color);

	virtual Color GetAmbientColor(const ShadingInfo& shadingInfo) const override;
	virtual Color GetDirectColor(ShadingInfo& shadingInfo) const override;
	virtual Color GetColorForAreaLight(ShadingInfo& shadingInfo) const override;
private:
	LambertianBRDF ambientBRDF;
	LambertianBRDF diffuseBRDF;

	Color deadColor;
};
