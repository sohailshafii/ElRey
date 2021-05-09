
#include "GlossySpecularMaterial.h"
#include "Sampling/GenericSampler.h"
#include "CommonMath.h"
#include <vector>

GlossySpecularMaterial::GlossySpecularMaterial(float ka, float kd, float ks,
											   float exponent,
											   std::shared_ptr<AbstractTexture> const & color,
											   std::shared_ptr<AbstractTexture> const & ksColor,
											   float cr, float kr)
	: PhongMaterial(ka, kd, ks, exponent, color, ksColor) {
	this->reflectivity = cr*kr;
}

void GlossySpecularMaterial::GetSecondaryVectors(ShadingInfo const & shadingInfo,
												 std::vector<SecondaryVectorInfo> & secondaryVectors) const {
	float rDotIncomingNormal;
	Vector3 reflectedVec =
		glossySpecularBRDF.GetReflectionVectorSampled(shadingInfo.wo, shadingInfo.normalVector, rDotIncomingNormal);
	Color3 directColor = GetDirectColor(shadingInfo);
	secondaryVectors.push_back(SecondaryVectorInfo(reflectedVec, reflectivity,
												   directColor));
}
