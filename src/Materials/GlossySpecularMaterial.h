#pragma once

#include "Material.h"
#include "Color.h"
#include "Materials/BRDFs/LambertianBRDF.h"
#include "Materials/BRDFs/GlossySpecularBRDF.h"

class GenericSampler;

class GlossySpecularMaterial : public Material {
public:
	GlossySpecularMaterial(float ka, float kd, float ks, float exponent,
						   const Color3& color, const Color3& ksColor, float cr, float kr);

	virtual Color GetAmbientColor(const ShadingInfo& shadingInfo) const override;
	virtual Color GetDirectColor(ShadingInfo& shadingInfo) const override;
	virtual Color GetColorForAreaLight(ShadingInfo& shadingInfo) const override;
	
	virtual void SetSampler(class GenericSampler *sampler) override;
	
	virtual bool DoesSurfaceReflect() const override {
		return true;
	}
	
	virtual Vector3 ReflectVectorOffSurface(Vector3 const &normal,
											Vector3 const &incomingVecFacingAwaySurface) const override;
	
	virtual float GetReflectivity() const override {
		return kr*cr;
	}
	
private:
	LambertianBRDF ambientBRDF;
	LambertianBRDF diffuseBRDF;
	GlossySpecularBRDF glossySpecularBRDF;

	Color deadColor;
	float kr, cr;
	float exponent;
};


