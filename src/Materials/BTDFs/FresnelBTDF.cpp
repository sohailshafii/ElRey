#include "FresnelBTDF.h"
#include "Sampling/GenericSampler.h"
#include "Math/CommonMath.h"
#include "Materials/Texturing/SingleColorTex.h"

FresnelBTDF::FresnelBTDF() : eta(0.0f),
	normalColor(std::make_shared<SingleColorTex>(SingleColorTex::SolidWhite())),
	sampler(nullptr) {
}

FresnelBTDF::FresnelBTDF(std::shared_ptr<AbstractTexture> const & color, float eta) {
	this->normalColor = color;
	this->eta = eta;
	this->sampler = nullptr;
}

// TODO make sure there are no memory errors, run through Dr Memory
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

Color3 FresnelBTDF::SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 						&transmittedVec, float& transmission, float etaOut) const {
	auto normal = shadingInfo.normalVector;
		
	float kr = 0.0f, kt = 0.0f;
	float relativeEta, cosThetaI, cosThetaT;
	Vector3 incomingVec = shadingInfo.wo;
	CommonMath::ComputeFresnelCoefficients(incomingVec, eta, etaOut, normal,
										   kr, kt, relativeEta, cosThetaI, cosThetaT);
	
	float invRelEta = 1.0f/relativeEta;
	transmittedVec = -incomingVec*invRelEta - normal*(cosThetaT - cosThetaI*invRelEta);
	transmission = kt;
	
	return normalColor->GetColor(shadingInfo)*invRelEta*invRelEta;
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

