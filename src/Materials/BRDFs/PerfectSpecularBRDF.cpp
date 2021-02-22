#include "PerfectSpecularBRDF.h"

PerfectSpecularBRDF::PerfectSpecularBRDF() : kr(0), cr(Color3(0.0f, 0.0f, 0.0f)),
	crScaled(cr*kr) {
}
	
PerfectSpecularBRDF::PerfectSpecularBRDF(float kr, Color3 cr) : kr(kr), cr(cr), crScaled(cr*kr) {
}

// TODO: not clear that a BRDF is supposed to modify wi! that's bad
Color3 PerfectSpecularBRDF::F(ShadingInfo& shadingInfo) const {
	Color3 	finalColor;
	Vector3 const & intersectionNormal = shadingInfo.normalVector;
	Vector3 const & wo = shadingInfo.wo;
	float ndotwo = intersectionNormal*wo;
	shadingInfo.wi = -wo + intersectionNormal * 2.0f * ndotwo;
	
	return (crScaled/fabs(intersectionNormal*shadingInfo.wi));
}

Color3 PerfectSpecularBRDF::SampleF(ShadingInfo& shadingInfo, float& pdf) const {
	Vector3 const & intersectionNormal = shadingInfo.normalVector;
	Vector3 const & wo = shadingInfo.wo;
	float ndotwo = intersectionNormal*wo;
	shadingInfo.wi = -wo + intersectionNormal * 2.0f * ndotwo;
	pdf = fabs(intersectionNormal*shadingInfo.wi);
	return crScaled;
}

Color3 PerfectSpecularBRDF::GetRho(const ShadingInfo& shadingInfo) const {
	return Color3::Black();
}


