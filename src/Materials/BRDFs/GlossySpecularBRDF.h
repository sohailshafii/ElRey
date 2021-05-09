#pragma once

#include "Materials/BRDFs/BRDF.h"
#include "Sampling/GenericSampler.h"
#include "Materials/Texturing/AbstractTexture.h"

class GlossySpecularBRDF : public BRDF {
public:
	GlossySpecularBRDF();
	GlossySpecularBRDF(GenericSampler *sampler, float ks,
					   std::shared_ptr<AbstractTexture> const & color, float exponent);
	
	~GlossySpecularBRDF();
	
	Vector3 GetReflectionVector(Vector3 const & wo,
								Vector3 const & normal) const;

	Color3 F(ShadingInfo const & shadingInfo) const override;
	
	Vector3 GetReflectionVectorSampled(Vector3 const & wo,
									   Vector3 const & normal,
									   float& rIncomingDotNormal) const;
	
	Color3 SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &newWi) const override;
	Color3 GetRho(const ShadingInfo& shadingInfo) const override;

	void setKs(float ks) {
		this->ks = ks;
	}

	void setCs(std::shared_ptr<AbstractTexture> const & cs) {
		this->cs = cs;
	}
	
	void setExponent(float exponent) {
		this->exponent = exponent;
	}
	
	void setSampler(GenericSampler *sampler);
	
	GenericSampler* GetSampler() const {
		return sampler;
	}

private:
	float ks;
	std::shared_ptr<AbstractTexture> cs;
	float exponent;
	
	GenericSampler *sampler;
};

