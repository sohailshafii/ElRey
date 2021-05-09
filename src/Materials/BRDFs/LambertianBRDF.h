#pragma once

#include "Materials/BRDFs/BRDF.h"
#include "Math/CommonMath.h"
#include "Sampling/GenericSampler.h"
#include "Materials/Texturing/AbstractTexture.h"
#include <memory>

class LambertianBRDF : public BRDF {
public:
	LambertianBRDF();
	LambertianBRDF(GenericSampler *sampler, float samplerExp, float kd,
				   std::shared_ptr<AbstractTexture> const & cd);
	
	~LambertianBRDF();

	Color3 F(ShadingInfo const & shadingInfo) const override;
	Color3 SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &newWi) const override;
	Color3 GetRho(const ShadingInfo& shadingInfo) const override;

	void setKd(float kd) {
		this->kd = kd;
		this->kdScaled = kd*INV_PI;
	}

	void setCd(std::shared_ptr<AbstractTexture> const & cd) {
		this->cd = cd;
	}
	
	void setSampler(GenericSampler *sampler);
	
	GenericSampler* GetSampler() {
		return sampler;
	}

private:
	float kd, kdScaled;
	std::shared_ptr<AbstractTexture> cd;
	
	GenericSampler *sampler;
};
