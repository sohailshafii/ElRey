#pragma once

#include "Material.h"
#include "Color.h"
#include "Materials/PhongMaterial.h"
#include "Materials/BRDFs/PerfectSpecularBRDF.h"

class GenericSampler;

class ReflectiveMaterial : public PhongMaterial {
public:
	ReflectiveMaterial(float ka, float kd, float ks, float exponent, const Color3& color,
					   const Color3& ksColor, float cr, float kr);
	
	virtual void SetSampler(class GenericSampler *sampler) override;
	
	virtual Color GetDirectColor(ShadingInfo& shadingInfo) const override;
	
	virtual void GetSecondaryVectors(ShadingInfo const & shadingInfo,
									 std::vector<SecondaryVectorInfo> & secondaryVectors) const override;
	
private:
	PerfectSpecularBRDF perfectSpecularBRDF;
	float reflectivity;
};

