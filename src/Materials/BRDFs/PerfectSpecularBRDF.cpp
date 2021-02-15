#include "PerfectSpecularBRDF.h"

PerfectSpecularBRDF::PerfectSpecularBRDF() : kr(0), cr(Color3(0.0f, 0.0f, 0.0f)),
	crScaled(cr*kr) {
}
	
PerfectSpecularBRDF::PerfectSpecularBRDF(float kr, Color3 cr) {
	this->kr = kr;
	this->cr = cr;
	this->crScaled = cr*kr;
}

Color3 PerfectSpecularBRDF::GetRadiance(ShadingInfo& shadingInfo) const {
	Color3 	finalColor;
	Vector3 const & intersectionNormal = shadingInfo.normalVector;
	Vector3 const & outgoingDir = shadingInfo.rayDirection;
	float ndotwo = intersectionNormal*outgoingDir;
	shadingInfo.incomingDirInverse = -outgoingDir +
		intersectionNormal * 2.0f * ndotwo;
	
	return (crScaled/fabs(intersectionNormal*shadingInfo.incomingDirInverse));
}

Color3 PerfectSpecularBRDF::GetRho(const ShadingInfo& shadingInfo) const {
	return Color3::Black();
}


