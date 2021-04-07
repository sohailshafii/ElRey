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
	auto incomingVec = shadingInfo.wo;
	float cosThetaI = normal * incomingVec;
	float relativeEta;
		
	if (cosThetaI < 0.0) {
		cosThetaI = -cosThetaI;
		normal = -normal;
		relativeEta = etaOut / eta;
	}
	else {
		relativeEta = eta / etaOut;
	}

	// compute transmitted coefficient before computing the related vector
	float descriminant = ComputeDescriminant(cosThetaI, 1.0f/relativeEta);
	float cosThetaT = sqrt(descriminant);
	float rParallel = (relativeEta * cosThetaI - cosThetaT) / (relativeEta * cosThetaI + cosThetaT);
	float rPerpendicular = (cosThetaI - relativeEta * cosThetaT) / (cosThetaI + relativeEta * cosThetaT);
	float kr = 0.5 * (rParallel * rParallel + rPerpendicular * rPerpendicular);
	float kt = 1.0f - kr;
	
	float invRelEta = 1.0f/relativeEta;
	transmittedVec = -incomingVec*invRelEta - normal*(cosThetaT - cosThetaI*invRelEta);
	transmission = kt*invRelEta*invRelEta;
	
	return normalColor;
}

bool FresnelBTDF::AllowsTransmission(ShadingInfo const & shadingInfo,
									 float etaOut) const {
	auto incomingVec = shadingInfo.wo;
	float cosTheta = shadingInfo.normalVector*incomingVec;
	float mag1 = shadingInfo.normalVector.Norm();
	float mag2 = incomingVec.Norm();
	// flip if are tracing from inside translucent object toward outside
	float invRelEta = cosTheta > 0.0f ? etaOut/eta : eta/etaOut;
	float descriminant = ComputeDescriminant(cosTheta, invRelEta);
	
	return descriminant >= 0.0f;
}

