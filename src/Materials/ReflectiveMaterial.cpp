#include "ReflectiveMaterial.h"
#include "Sampling/GenericSampler.h"
#include <vector>

ReflectiveMaterial::ReflectiveMaterial(float ka, float kd, float ks, float exponent,
									   const Color3& color, const Color3& ksColor,
									   float cr, float kr) : PhongMaterial(ka, ka, ks, exponent, color, ksColor) {
	perfectSpecularBRDF.setKs(ks);
	perfectSpecularBRDF.setExponent(exponent);
	perfectSpecularBRDF.setCs(ksColor);

	this->reflectivity = cr*kr;
}

void ReflectiveMaterial::SetSampler(GenericSampler *sampler) {
	PhongMaterial::SetSampler(sampler->clone());
	perfectSpecularBRDF.setSampler(sampler);
}

void ReflectiveMaterial::GetSecondaryVectors(ShadingInfo const & shadingInfo,
											 std::vector<SecondaryVectorInfo> & secondaryVectors) const {
	Vector3 reflectedVec =
		perfectSpecularBRDF.GetReflectionVector(shadingInfo.wo,
												shadingInfo.normalVector);
	secondaryVectors.push_back(SecondaryVectorInfo(reflectedVec, reflectivity,
												   Color::Black()));
}
