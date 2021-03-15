#pragma once

#include "Material.h"
#include "Color.h"
#include "Materials/BRDFs/LambertianBRDF.h"
#include "Materials/BRDFs/GlossySpecularBRDF.h"

class PhongMaterial : public Material {
public:
	// TODO: mismatch between Color4 and Color3 -- should we deal with transparency in BRDFs?
	PhongMaterial(float ka, float kd, float ks, float exponent, const Color3& color,
				  const Color3& ksColor);

	virtual Color GetAmbientColor(const ShadingInfo& shadingInfo) const override;
	virtual Color GetDirectColor(ShadingInfo& shadingInfo) const override;
	virtual Color SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<float>& pdfs, std::vector<Vector3> &wis) const override;
	
	virtual void SetSampler(class GenericSampler *sampler) override;
	
private:
	LambertianBRDF ambientBRDF;
	LambertianBRDF diffuseBRDF;
	GlossySpecularBRDF glossySpecularBRDF;

	Color deadColor;
};

