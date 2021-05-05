#pragma once

#include "Materials/BTDFs/BTDF.h"
#include "Sampling/GenericSampler.h"
#include "Materials/Color3.h"

class FresnelBTDF : public BTDF {
public:
	FresnelBTDF();
	FresnelBTDF(Color3 const & color, float eta);
	~FresnelBTDF();
	
	Color3 SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &transmittedVec,
				   float &transmission, float etaOut = 1.0f) const override;
	
	bool AllowsTransmission(ShadingInfo const & shadingInfo,
							float etaOut = 1.0f) const override;
	
	void SetColor(Color3 const & color) {
		this->normalColor = color;
	}
	
	void SetEta(float eta) {
		this->eta = eta;
	}
	
	void SetExponent(float exponent) {
		this->exponent = exponent;
	}
	
	void SetSampler(GenericSampler *sampler);
	
	GenericSampler* GetSampler() {
		return sampler;
	}

private:
	Color3 normalColor;
	float eta;
	float exponent;
	
	GenericSampler *sampler;
};

