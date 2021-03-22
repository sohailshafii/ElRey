#include "TransparentMaterial.h"
#include "Sampling/GenericSampler.h"
#include <vector>

TransparentMaterial::TransparentMaterial(float ka, float kd, float ks, float exponent,
										 const Color3& color, const Color3& ksColor,
										 float cr, float kr, float eta, float kt) : ReflectiveMaterial(ka, kd, ks, exponent, color, ksColor, cr, kr) {
	perfectTransmitterBTDF.SetEta(eta);
	perfectTransmitterBTDF.SetKt(kt);
}

void TransparentMaterial::GetSecondaryVectors(ShadingInfo const & shadingInfo,
											  std::vector<SecondaryVectorInfo> & secondaryVectors) const {
	ReflectiveMaterial::GetSecondaryVectors(shadingInfo, secondaryVectors);
	if (perfectTransmitterBTDF.AllowsTransmission(shadingInfo)) {
		float pdf;
		Vector3 transmittedVec;
		float transmission;
		Color transmittedColor = perfectTransmitterBTDF.SampleF(
										shadingInfo, pdf, transmittedVec,
										transmission);
		secondaryVectors.push_back(SecondaryVectorInfo(transmittedVec, transmission, transmittedColor));
	}
	else {
		// crank up reflectivity to 100%
		secondaryVectors[0].vecCoeff = 1.0f;
	}
}
