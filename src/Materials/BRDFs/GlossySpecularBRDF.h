#pragma once

#include "Materials/BRDFs/BRDF.h"
#include "Sampling/GenericSampler.h"

class GlossySpecularBRDF : public BRDF {
public:
	GlossySpecularBRDF();
	GlossySpecularBRDF(GenericSampler *sampler, float ks, Color3 cs, float exponent);
	
	~GlossySpecularBRDF();
	
	Vector3 GetReflectionVector(Vector3 const & wo,
								Vector3 const & normal) const;

	Color3 F(ShadingInfo& shadingInfo) const override;
	
	Vector3 GetReflectionVectorSampled(Vector3 const & wo,
									   Vector3 const & normal,
									   float& rIncomingDotNormal) const;
	
	Color3 SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &newWi) const override;
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
	
	void setSampler(GenericSampler *sampler);
	
	GenericSampler* GetSampler() const {
		return sampler;
	}

private:
	float ks;
	Color3 cs;
	Color3 csScaled;
	float exponent;
	
	GenericSampler *sampler;
};

