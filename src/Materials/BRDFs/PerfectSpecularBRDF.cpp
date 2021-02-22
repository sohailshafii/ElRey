#include "PerfectSpecularBRDF.h"

PerfectSpecularBRDF::PerfectSpecularBRDF() : kr(0), cr(Color3(0.0f, 0.0f, 0.0f)),
	crScaled(cr*kr) {
}
	
PerfectSpecularBRDF::PerfectSpecularBRDF(float kr, Color3 cr) : kr(kr), cr(cr), crScaled(cr*kr) {
}

Color3 PerfectSpecularBRDF::F(ShadingInfo& shadingInfo) const {
	Color3 	finalColor;
	Vector3 const & intersectionNormal = shadingInfo.normalVector;
	Vector3 const & outgoingDir = shadingInfo.incomingDirInverse;
	float ndotwo = intersectionNormal*outgoingDir;
	// TODO: call wi!!
	shadingInfo.vectorToLight = -outgoingDir +
		intersectionNormal * 2.0f * ndotwo;
	
	return (crScaled/fabs(intersectionNormal*shadingInfo.incomingDirInverse));
}

Color3 PerfectSpecularBRDF::SampleF(ShadingInfo& shadingInfo, float& pdf) const {
	Vector3 const & intersectionNormal = shadingInfo.normalVector;
	Vector3 const & outgoingDir = shadingInfo.incomingDirInverse;
	float ndotwo = intersectionNormal*outgoingDir;
	// TODO: call wi!!
	shadingInfo.vectorToLight = -outgoingDir +
		intersectionNormal * 2.0f * ndotwo;
	
	return crScaled;
}

Color3 PerfectSpecularBRDF::GetRho(const ShadingInfo& shadingInfo) const {
	return Color3::Black();
}


