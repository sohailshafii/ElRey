#pragma once

#include "Materials/BTDFs/BTDF.h"
#include "Sampling/GenericSampler.h"

class PerfectTransmitterBTDF : public BTDF {
public:
	PerfectTransmitterBTDF();
	PerfectTransmitterBTDF(float eta, float kt);
	~PerfectTransmitterBTDF();
	
	Color3 SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &transmittedVec,
				   float &transmission, float etaOut = 1.0f) const override;
	
	bool AllowsTransmission(ShadingInfo const & shadingInfo,
							float etaOut = 1.0f) const override;
	
	void SetEta(float eta) {
		this->eta = eta;
	}
	
	void SetKt(float kt) {
		this->kt = kt;
	}

private:
	Color3 normalColor;
	float eta;
	float kt;
};

