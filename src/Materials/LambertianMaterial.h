#pragma once

#include "Material.h"
#include "Color.h"
#include "Materials/BRDFs/LambertianBRDF.h"

class LambertianMaterial : public Material {
public:
	LambertianMaterial(float ka, float kd, const Color3& color);

	virtual Color3 GetAmbientColor(const ShadingInfo& shadingInfo) const override;
	virtual Color3 GetDirectColor(ShadingInfo const & shadingInfo) const override;
	virtual void SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<DirectionSample> & directionSamples) const override;
	
	virtual void SetSampler(class GenericSampler *sampler) override;
	
private:
	LambertianBRDF ambientBRDF;
	LambertianBRDF diffuseBRDF;

	Color3 deadColor;
};
