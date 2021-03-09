#pragma once

#include "Materials/BRDFs/BRDF.h"
#include "Math/CommonMath.h"

class GenericSampler;

class PerfectSpecularBRDF : public BRDF {
public:
	PerfectSpecularBRDF();
	PerfectSpecularBRDF(GenericSampler *sampler, float ks, Color3 cs, float exponent);
	
	~PerfectSpecularBRDF();

	Vector3 GetReflectionVector(Vector3 const & wo,
								Vector3 const & normal) const;
	
	Color3 F(ShadingInfo& shadingInfo) const override;
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
	
	void setSampler(GenericSampler *sampler) {
		this->sampler = sampler;
	}
	
	GenericSampler* GetSampler() {
		return sampler;
	}

private:
	float ks;
	Color3 cs;
	Color3 csScaled;
	float exponent;
	
	GenericSampler *sampler;
};

