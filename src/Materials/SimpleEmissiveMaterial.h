#pragma once

#include "Material.h"
#include "Color3.h"

class SimpleEmissiveMaterial : public Material {
public:
	SimpleEmissiveMaterial(float ka, float kd, const Color3& color);

	virtual Color3 GetAmbientColor(const ShadingInfo &shadingInfo) const override;
	virtual Color3 GetDirectColor(ShadingInfo const &shadingInfo) const override;
	virtual void SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<DirectionSample> & directionSamples) const override;
	
	virtual void SetSampler(class GenericSampler *sampler) override;
	
private:
	Color3 ambientColor;
	Color3 directColor;

	Color3 deadColor;
};
