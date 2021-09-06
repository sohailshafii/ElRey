#pragma once

#include "Materials/BRDFs/BRDF.h"
#include "Math/CommonMath.h"
#include "Sampling/GenericSampler.h"
#include <memory>

class AbstractTexture;

class FresnelBRDF : public BRDF {
public:
	FresnelBRDF();
	FresnelBRDF(GenericSampler *sampler,
				std::shared_ptr<AbstractTexture> const & cs,
				float exponent, float eta, float etaOut);
	
	~FresnelBRDF();

	Vector3 GetReflectionVector(Vector3 const & wo,
								Vector3 const & normal) const;
	
	Color3 F(ShadingInfo const & shadingInfo) const override;
	Color3 SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &newWi) const override;
	Color3 GetRho(const ShadingInfo& shadingInfo) const override;

	void SetCs(std::shared_ptr<AbstractTexture> const & cs) {
		this->cs = cs;
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
	std::shared_ptr<AbstractTexture> cs;
	float exponent;
	float eta, etaOut;
	
	GenericSampler *sampler;
};


