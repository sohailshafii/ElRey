#pragma once

#include "Material.h"
#include "Color3.h"
#include "Materials/BRDFs/LambertianBRDF.h"
#include "Materials/BRDFs/GlossySpecularBRDF.h"
#include "Materials/Texturing/AbstractTexture.h"

class PhongMaterial : public Material {
public:
	PhongMaterial(float ka, float kd, float ks, float exponent,
				  std::shared_ptr<AbstractTexture> const & color,
				  std::shared_ptr<AbstractTexture> const & ksColor);

	virtual Color3 GetAmbientColor(const ShadingInfo& shadingInfo) const override;
	virtual Color3 GetDirectColor(ShadingInfo const & shadingInfo) const override;
	virtual void SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<DirectionSample> & directionSamples) const override;
	
	virtual void SetSampler(class GenericSampler *sampler) override;
	
protected:
	LambertianBRDF ambientBRDF;
	LambertianBRDF diffuseBRDF;
	GlossySpecularBRDF glossySpecularBRDF;

	Color3 deadColor;
};

