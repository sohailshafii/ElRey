#pragma once

#include "Materials/BTDFs/BTDF.h"
#include "Sampling/GenericSampler.h"

class FresnelBTDF : public BTDF {
public:
	FresnelBTDF();
	FresnelBTDF(float eta);
	~FresnelBTDF();
	
	Color SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &transmittedVec,
				  float &transmission, float etaOut = 1.0f) const override;
	
	bool AllowsTransmission(ShadingInfo const & shadingInfo,
							float etaOut = 1.0f) const override;
	
	void SetEta(float eta) {
		this->eta = eta;
	}

private:
	Color normalColor;
	float eta;
};

