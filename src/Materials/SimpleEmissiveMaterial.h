#pragma once

#include "Material.h"
#include "Color3.h"

class SimpleEmissiveMaterial : public Material {
public:
	SimpleEmissiveMaterial(float ka, float kd, const Color3& color);

	virtual Color GetAmbientColor(const ShadingInfo &shadingInfo) const override;
	virtual Color GetDirectColor(ShadingInfo &shadingInfo) const override;
	virtual Color GetColorForAreaLight(ShadingInfo &shadingInfo) const override;
private:
	Color ambientColor;
	Color directColor;

	Color deadColor;
};
