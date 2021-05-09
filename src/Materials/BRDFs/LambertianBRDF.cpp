#include "LambertianBRDF.h"
#include "Sampling/GenericSampler.h"
#include "Math/CommonMath.h"

LambertianBRDF::LambertianBRDF() : sampler(nullptr),
	kd(0), kdScaled(0), cd(nullptr) {
}
	
LambertianBRDF::LambertianBRDF(GenericSampler *sampler,
							   float samplerExp, float kd,
							   std::shared_ptr<AbstractTexture> const & cd)
	: sampler(sampler), kd(kd), kdScaled(kd*INV_PI),
		cd(cd) {
	sampler->MapSamplesToHemisphere(samplerExp);
}

LambertianBRDF::~LambertianBRDF() {
	if (sampler != nullptr) {
		delete sampler;
	}
}

void LambertianBRDF::setSampler(GenericSampler *sampler) {
	if (this->sampler != nullptr) {
		   delete sampler;
	}
	this->sampler = sampler;
	this->sampler->MapSamplesToHemisphere(1.0f);
}

Color3 LambertianBRDF::F(ShadingInfo const & shadingInfo) const {
	return cd->GetColor(shadingInfo)*kdScaled;
}

Color3 LambertianBRDF::SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &newWi) const {
	Vector3 w = shadingInfo.normalVector;
	Vector3 u = Vector3(0.00424f, 1.0f, 0.00764f) ^ w;
	u.Normalize();
	Vector3 v = w ^ u;
	v.Normalize();
	CommonMath::ComputeUVWFromUpandForward(u, v, w);
	
	Point3 samplePoint = sampler->GetSampleOnHemisphere();
	newWi = u*samplePoint[0] + v*samplePoint[1] + w*samplePoint[2];
	newWi.Normalize();
	
	pdf = shadingInfo.normalVector * newWi * (float)INV_PI;
	
	return cd->GetColor(shadingInfo)*kdScaled;
}

Color3 LambertianBRDF::GetRho(const ShadingInfo& shadingInfo) const {
	return cd->GetColor(shadingInfo)*kd;
}
