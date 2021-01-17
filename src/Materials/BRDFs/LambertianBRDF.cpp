#include "LambertianBRDF.h"

LambertianBRDF::LambertianBRDF() : kd(0), cd(Color3(0.0f, 0.0f, 0.0f)),
	cdScaled(cd*kd), uniformRadiance(cdScaled*INV_PI) {
}
	
LambertianBRDF::LambertianBRDF(float kd, const Color3& cd) {
	this->kd = kd;
	this->cd = cd;
	this->cdScaled = cd*kd;
	this->uniformRadiance = this->cdScaled*INV_PI;
}

Color3 LambertianBRDF::GetRadiance(const ShadingInfo& shadingInfo) const {
	return this->uniformRadiance;
}

Color3 LambertianBRDF::GetRho(const ShadingInfo& shadingInfo) const {
	return this->cdScaled;
}
