
#include "GlossySpecularMaterial.h"
#include "Sampling/GenericSampler.h"
#include "CommonMath.h"
#include <vector>

GlossySpecularMaterial::GlossySpecularMaterial(float ka, float kd, float ks,
											   float exponent, const Color3& color,
											   const Color3& ksColor, float cr, float kr)
	: PhongMaterial(ka, kd, ks, exponent, color, ksColor) {
	this->reflectivity = cr*kr;
}

void GlossySpecularMaterial::GetSecondaryVectors(ShadingInfo const & shadingInfo,
												 Vector3 const &normal,
												 Vector3 const &incomingVecFacingAwaySurface,
												 std::vector<SecondaryVectorInfo> & secondaryVectors) const {
	float rDotIncomingNormal;
	Vector3 reflectedVec =
		glossySpecularBRDF.GetReflectionVectorSampled(incomingVecFacingAwaySurface, normal, rDotIncomingNormal);
	secondaryVectors.push_back(SecondaryVectorInfo(reflectedVec, reflectivity));
}
