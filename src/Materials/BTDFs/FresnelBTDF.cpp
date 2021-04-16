#include "FresnelBTDF.h"
#include "Sampling/GenericSampler.h"
#include "Math/CommonMath.h"

FresnelBTDF::FresnelBTDF() : eta(0.0f), normalColor(Color::White()) {
}

FresnelBTDF::FresnelBTDF(Color3 const & color, float eta) {
	this->normalColor = Color(color[0], color[1], color[2], 1.0f);
	this->eta = eta;
}

FresnelBTDF::~FresnelBTDF() {
	if (sampler != nullptr) {
		delete sampler;
	}
}

void FresnelBTDF::SetSampler(GenericSampler *sampler) {
	if (this->sampler != nullptr) {
		delete sampler;
	}
	this->sampler = sampler;
	this->sampler->MapSamplesToHemisphere(exponent);
}

Color FresnelBTDF::SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 									&transmittedVec, float& transmission, float etaOut) const {
	auto normal = shadingInfo.normalVector;
		
	float kr = 0.0f, kt = 0.0f;
	float relativeEta, cosThetaI, cosThetaT;
	Vector3 incomingVec = shadingInfo.wo;
	CommonMath::ComputeFresnelCoefficients(incomingVec, eta, etaOut, normal,
										   kr, kt, relativeEta, cosThetaI, cosThetaT);
	
	float invRelEta = 1.0f/relativeEta;
	transmittedVec = -incomingVec*invRelEta - normal*(cosThetaT - cosThetaI*invRelEta);
	transmission = kt;
	
	return normalColor*invRelEta*invRelEta;
}

bool FresnelBTDF::AllowsTransmission(ShadingInfo const & shadingInfo,
									 float etaOut) const {
	auto incomingVec = shadingInfo.wo;
	float cosTheta = shadingInfo.normalVector*incomingVec;
	// flip if are tracing from inside translucent object toward outside
	float invRelEta = cosTheta > 0.0f ? etaOut/eta : eta/etaOut;
	float descriminant = CommonMath::ComputeFresnelDescriminant(cosTheta, invRelEta);
	
	return descriminant >= 0.0f;
}

