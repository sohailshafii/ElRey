#pragma once

#include "Material.h"
#include "Color3.h"

class SimpleEmissiveMaterial : public Material {
public:
	SimpleEmissiveMaterial(float ka, float kd, const Color3& color);

	virtual Color GetAmbientColor(const IntersectionResult &intersectionResult) const override;
	virtual Color GetDirectColor(const IntersectionResult &intersectionResult) const override;
	virtual Color GetColorForAreaLight(const IntersectionResult& intersectionResult) const override;
private:
	Color ambientColor;
	Color directColor;

	Color deadColor;
};
