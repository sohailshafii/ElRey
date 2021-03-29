#include "PerfectTransmitterBTDF.h"
#include "Sampling/GenericSampler.h"
#include "Math/CommonMath.h"

PerfectTransmitterBTDF::PerfectTransmitterBTDF() : eta(0.0f), kt(0.0f), normalColor(Color::White()) {
}

PerfectTransmitterBTDF::PerfectTransmitterBTDF(float eta, float kt) {
	this->eta = eta;
	this->kt = kt;
	normalColor = Color::White();
}

PerfectTransmitterBTDF::~PerfectTransmitterBTDF() {
}

Color PerfectTransmitterBTDF::SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 									&transmittedVec, float& transmission, float etaOut) const {
	auto normal = shadingInfo.normalVector;
	auto incomingVec = shadingInfo.wo;
	float cosTheta = normal * incomingVec;
	float invRelEta = etaOut/eta;
	
	if (cosTheta < 0.0f) {
		cosTheta = -cosTheta;
		normal = -normal;
		invRelEta = eta/etaOut;
	}
	
	float descSqrt = sqrt(ComputeDescriminant(cosTheta, invRelEta));
	
	transmittedVec = -incomingVec*invRelEta - normal*(descSqrt - cosTheta*invRelEta);
	transmission = kt*invRelEta*invRelEta;
	
	return normalColor;
}

bool PerfectTransmitterBTDF::AllowsTransmission(ShadingInfo const & shadingInfo,
												float etaOut) const {
	if (kt <= EPSILON) {
		return false;
	}
	auto incomingVec = shadingInfo.wo;
	float cosTheta = shadingInfo.normalVector*incomingVec;
	float mag1 = shadingInfo.normalVector.Norm();
	float mag2 = incomingVec.Norm();
	// flip if are tracing from inside translucent object toward outside
	float invRelEta = cosTheta > 0.0f ? etaOut/eta : eta/etaOut;
	float descriminant = ComputeDescriminant(cosTheta, invRelEta);
	
	return descriminant >= 0.0f;
}

