#include "FresnelBTDF.h"
#include "Sampling/GenericSampler.h"
#include "Math/CommonMath.h"

FresnelBTDF::FresnelBTDF() : eta(0.0f), normalColor(Color::White()) {
}

FresnelBTDF::FresnelBTDF(float eta) {
	this->eta = eta;
	normalColor = Color::White();
}

FresnelBTDF::~FresnelBTDF() {
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
	transmission = kt*invRelEta*invRelEta;
	
	return normalColor;
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

