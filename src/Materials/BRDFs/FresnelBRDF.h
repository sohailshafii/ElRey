#pragma once

#include "Materials/BRDFs/BRDF.h"
#include "Math/CommonMath.h"
#include "Sampling/GenericSampler.h"

class FresnelBRDF : public BRDF {
public:
	FresnelBRDF();
	FresnelBRDF(GenericSampler *sampler, float ks, Color3 cs, float exponent,
				float eta, float etaOut);
	
	~FresnelBRDF();

	Vector3 GetReflectionVector(Vector3 const & wo,
								Vector3 const & normal) const;
	
	Color3 F(ShadingInfo const & shadingInfo) const override;
	Color3 SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &newWi) const override;
	Color3 GetRho(const ShadingInfo& shadingInfo) const override;
	
	void SetKs(float ks) {
		this->ks = ks;
		this->csScaled = cs*ks;
	}

	void SetCs(const Color3& cs) {
		this->cs = cs;
		this->csScaled = cs*ks;
	}
	
	void SetExponent(float exponent) {
		this->exponent = exponent;
	}
	
	void SetEtaIn(float eta) {
		this->eta = eta;
	}
	
	void SetEtaOut(float etaOut) {
		this->etaOut = etaOut;
	}
	
	void SetSampler(GenericSampler *sampler);
	
	GenericSampler* GetSampler() {
		return sampler;
	}

private:
	float ks;
	Color3 cs;
	Color3 csScaled;
	float exponent;
	float eta, etaOut;
	
	GenericSampler *sampler;
	
	float ComputeFresnelCoeff(ShadingInfo const & shadingInfo);
	
	float ComputeDescriminant(float cosTheta, float invRelEta) const {
		return 1.0f - invRelEta*invRelEta*(1.0f - cosTheta*cosTheta);
	}
};


