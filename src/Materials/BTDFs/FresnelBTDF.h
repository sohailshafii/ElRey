#pragma once

#include "Materials/BTDFs/BTDF.h"
#include "Sampling/GenericSampler.h"
#include "Materials/Color3.h"

class AbstractTexture;

class FresnelBTDF : public BTDF {
public:
	FresnelBTDF();
	FresnelBTDF(std::shared_ptr<AbstractTexture> const & color, float eta);
	~FresnelBTDF();
	
	Color3 SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &transmittedVec,
				   float &transmission, float etaOut = 1.0f) const override;
	
	bool AllowsTransmission(ShadingInfo const & shadingInfo,
							float etaOut = 1.0f) const override;
	
	void SetColor(std::shared_ptr<AbstractTexture> const & color) {
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
	std::shared_ptr<AbstractTexture> normalColor;
	float eta;
	float exponent;
	
	GenericSampler *sampler;
};

