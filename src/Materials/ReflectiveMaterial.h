#pragma once

#include "Material.h"
#include "Color.h"
#include "Materials/BRDFs/LambertianBRDF.h"
#include "Materials/BRDFs/GlossySpecularBRDF.h"
#include "Materials/BRDFs/PerfectSpecularBRDF.h"

class GenericSampler;

class ReflectiveMaterial : public Material {
public:
	ReflectiveMaterial(float ka, float kd, float ks, float exponent, const Color3& color,
					   const Color3& ksColor, float cr, float kr);

	virtual Color GetAmbientColor(const ShadingInfo& shadingInfo) const override;
	virtual Color GetDirectColor(ShadingInfo& shadingInfo) const override;
	virtual void SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<DirectionSample> & directionSamples) const override;
	
	virtual void SetSampler(class GenericSampler *sampler) override;
	
	virtual void GetSecondaryVectors(Vector3 const &normal,
									 Vector3 const &incomingVecFacingAwaySurface,
									 std::vector<SecondaryVectorInfo> & secondaryVectors) const override;
	
private:
	LambertianBRDF ambientBRDF;
	LambertianBRDF diffuseBRDF;
	GlossySpecularBRDF glossySpecularBRDF;
	PerfectSpecularBRDF perfectSpecularBRDF;

	Color deadColor;
	float reflectivity;
};

