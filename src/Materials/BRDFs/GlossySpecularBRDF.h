#pragma once

#include "Materials/BRDFs/BRDF.h"
#include "Math/CommonMath.h"

class GenericSampler;

class GlossySpecularBRDF : public BRDF {
public:
	GlossySpecularBRDF();
	GlossySpecularBRDF(GenericSampler *sampler, float ks, Color3 cs, float exponent);
	
	~GlossySpecularBRDF();

	Color3 F(ShadingInfo& shadingInfo) const override;
	Color3 SampleF(ShadingInfo& shadingInfo, float& pdf) const override;
	Color3 GetRho(const ShadingInfo& shadingInfo) const override;

	void setKs(float ks) {
		this->ks = ks;
		this->csScaled = cs*ks;
	}

	void setCs(const Color3& cs) {
		this->cs = cs;
		this->csScaled = cs*ks;
	}
	
	void setExponent(float exponent) {
		this->exponent = exponent;
	}
	
	void setSampler(GenericSampler *sampler) {
		this->sampler = sampler;
	}

private:
	float ks;
	Color3 cs;
	Color3 csScaled;
	float exponent;
	
	GenericSampler *sampler;
};

