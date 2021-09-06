#pragma once

#include "Material.h"
#include "Color3.h"
#include <memory>

class AbstractTexture;

class SimpleEmissiveMaterial : public Material {
public:
	SimpleEmissiveMaterial(float ka, float kd, std::shared_ptr<AbstractTexture> const & color);

	virtual Color3 GetAmbientColor(const ShadingInfo &shadingInfo) const override;
	virtual Color3 GetDirectColor(ShadingInfo const &shadingInfo) const override;
	virtual void SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<DirectionSample> & directionSamples) const override;
	
	virtual void SetSampler(class GenericSampler *sampler) override;
	
private:
	float ka, kd;
	std::shared_ptr<AbstractTexture> color;

	Color3 deadColor;
};
