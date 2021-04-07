#include "DielectricMaterial.h"
#include "Sampling/GenericSampler.h"
#include <vector>

DielectricMaterial::DielectricMaterial(float ka, float kd, float ks, float exponent,
									   const Color3& color, const Color3& ksColor,
									   float etaIn, float etaOut,
									   Color3 const & cfIn, Color3 const & cfOut) :
		PhongMaterial(ka, kd, ks, exponent, color, ksColor) {
	perfectTransmitterBTDF.SetEta(etaIn);
	//perfectTransmitterBTDF.SetKt(kt);
	this->etaOut = etaOut;
	this->cfIn = cfIn;
	this->cfOut = cfOut;
}

void DielectricMaterial::GetSecondaryVectors(ShadingInfo const & shadingInfo,
											  std::vector<SecondaryVectorInfo> & secondaryVectors) const {
	if (perfectTransmitterBTDF.AllowsTransmission(shadingInfo)) {
		float pdf;
		Vector3 transmittedVec;
		float transmission;
		Color transmittedColor = perfectTransmitterBTDF.SampleF(
										shadingInfo, pdf, transmittedVec,
										transmission);
		secondaryVectors.push_back(SecondaryVectorInfo(transmittedVec, transmission, transmittedColor));
	}
	// only if reflective coefficent is strong
	else if (secondaryVectors.size() > 0) {
		// crank up reflectivity to 100%
		secondaryVectors[0].vecCoeff = 1.0f;
	}
}
