#pragma once

#include "Materials/BRDFs/BRDF.h"
#include "Math/CommonMath.h"
#include "Sampling/GenericSampler.h"

class LambertianBRDF : public BRDF {
public:
	LambertianBRDF();
	LambertianBRDF(GenericSampler *sampler, float samplerExp, float kd, const Color3& cd);
	
	~LambertianBRDF();

	Color3 F(ShadingInfo& shadingInfo) const override;
	Color3 SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &newWi) const override;
	Color3 GetRho(const ShadingInfo& shadingInfo) const override;

	void setKd(float kd) {
		this->kd = kd;
		this->cdScaled = cd*kd;
		this->uniformRadiance = this->cdScaled*INV_PI;
	}

	void setCd(const Color3& cd) {
		this->cd = cd;
		this->cdScaled = cd*kd;
		this->uniformRadiance = this->cdScaled*INV_PI;
	}
	
	void setSampler(GenericSampler *sampler);
	
	GenericSampler* GetSampler() {
		return sampler;
	}

private:
	float kd;
	Color3 cd;
	Color3 cdScaled;
	Color3 uniformRadiance;
	
	GenericSampler *sampler;
};
