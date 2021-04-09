#pragma once

#include "Materials/BTDFs/BTDF.h"
#include "Sampling/GenericSampler.h"
#include "Materials/Color3.h"

class FresnelBTDF : public BTDF {
public:
	FresnelBTDF();
	FresnelBTDF(Color3 const & color, float eta);
	~FresnelBTDF();
	
	Color SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &transmittedVec,
				  float &transmission, float etaOut = 1.0f) const override;
	
	bool AllowsTransmission(ShadingInfo const & shadingInfo,
							float etaOut = 1.0f) const override;
	
	void SetColor(Color3 const & color) {
		this->normalColor = Color(color[0], color[1], color[2], 1.0f);;
	}
	
	void SetEta(float eta) {
		this->eta = eta;
	}

private:
	Color normalColor;
	float eta;
};

