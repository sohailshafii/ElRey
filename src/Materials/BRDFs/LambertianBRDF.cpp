#include "LambertianBRDF.h"
#include "Sampling/GenericSampler.h"
#include "Math/CommonMath.h"

LambertianBRDF::LambertianBRDF() : sampler(nullptr),
	kd(0), cd(Color3(0.0f, 0.0f, 0.0f)),
	cdScaled(cd*kd), uniformRadiance(cd*kd*INV_PI) {
}
	
LambertianBRDF::LambertianBRDF(GenericSampler *sampler,
							   float samplerExp, float kd,
							   const Color3& cd)
	: sampler(sampler), kd(kd), cd(cd), cdScaled(cd*kd),
		uniformRadiance(cd*kd*INV_PI) {
	sampler->MapSamplesToHemisphere(samplerExp);
}

LambertianBRDF::~LambertianBRDF() {
	if (sampler != nullptr) {
		delete sampler;
	}
}

Color3 LambertianBRDF::F(ShadingInfo& shadingInfo) const {
	return this->uniformRadiance;
}

Color3 LambertianBRDF::SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &newWi) const {
	Vector3 w = shadingInfo.normalVector;
	Vector3 v = Vector3(0.00424, 1, 0.00764) ^ w;
	v.Normalize();
	Vector3 u;
	CommonMath::ComputeUVWFromWandU(u, v, w);
	
	Point3 samplePoint = sampler->GetSampleOnHemisphere();
	newWi = u*samplePoint[0] + v*samplePoint[1] + w*samplePoint[2];
	newWi.Normalize();
	
	pdf = shadingInfo.normalVector * newWi * INV_PI;
	
	return uniformRadiance;
}

Color3 LambertianBRDF::GetRho(const ShadingInfo& shadingInfo) const {
	return this->cdScaled;
}
