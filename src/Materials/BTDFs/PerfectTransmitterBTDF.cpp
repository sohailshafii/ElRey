#include "PerfectTransmitterBTDF.h"
#include "Sampling/GenericSampler.h"
#include "Math/CommonMath.h"

PerfectTransmitterBTDF::PerfectTransmitterBTDF() : eta(0.0f), kt(0.0f) {
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
	float cosTheta = normal * shadingInfo.wo;
	float relativeEta = eta/etaOut;
	
	if (cosTheta < 0.0f) {
		cosTheta = -cosTheta;
		normal = -normal;
		relativeEta = 1.0f/relativeEta;
	}
	
	float temp = 1.0f - (1.0f - cosTheta*cosTheta)/(relativeEta*relativeEta);
	float cosThetaSqrt = sqrt(temp);
	
	transmittedVec = shadingInfo.wo*(1.0f/relativeEta) -
		normal*((cosThetaSqrt - cosTheta)/relativeEta);
	transmittedVec.Normalize();
	transmission = kt /(relativeEta*relativeEta);
	
	return normalColor;
}

bool PerfectTransmitterBTDF::AllowsTransmission(ShadingInfo const & shadingInfo,
												float etaOut) const {
	float cosTheta = shadingInfo.normalVector*shadingInfo.wo;
	float relativeEta = cosTheta < 0.0f ? eta/etaOut : etaOut/eta;
	
	return (1.0f - (1.0f - cosTheta * cosTheta)/(relativeEta*relativeEta)) < 0.0f;
}

