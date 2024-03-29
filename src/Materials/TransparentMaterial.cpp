#include "TransparentMaterial.h"
#include "Sampling/GenericSampler.h"
#include <vector>

TransparentMaterial::TransparentMaterial(float ka, float kd, float ks,
										 float exponent,
										 std::shared_ptr<AbstractTexture> const & color,
										 std::shared_ptr<AbstractTexture> const & ksColor,
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
		Color3 transmittedColor = perfectTransmitterBTDF.SampleF(
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
