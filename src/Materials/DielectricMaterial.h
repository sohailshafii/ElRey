#pragma once

#include "Material.h"
#include "Color.h"
#include "Materials/ReflectiveMaterial.h"
#include "Materials/BRDFs/FresnelBRDF.h"
#include "Materials/BTDFs/FresnelBTDF.h"
#include "Materials/Texturing/AbstractTexture.h"

class GenericSampler;

class DielectricMaterial : public PhongMaterial {
public:
	DielectricMaterial(float ka, float kd, float ks, float exponent,
					   std::shared_ptr<AbstractTexture> const & color,
					   std::shared_ptr<AbstractTexture> const & ksColor,
					   float etaIn, float etaOut,
					   Color3 const & cfIn, Color3 const & cfOut);
	
	virtual void SetSampler(class GenericSampler *sampler) override;
	
	virtual void GetSecondaryVectors(ShadingInfo const & shadingInfo,
									 std::vector<SecondaryVectorInfo> & secondaryVectors) const override;
	
private:
	FresnelBRDF fresnelBrdf;
	FresnelBTDF fresnelBtdf;
	float etaOut;
	Color3 cfIn;
	Color3 cfOut;
};


