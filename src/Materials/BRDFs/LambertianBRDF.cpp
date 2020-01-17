#include "LambertianBRDF.h"

LambertianBRDF::LambertianBRDF() : kd(0), cd(Color(0.0f, 0.0f, 0.0f, 0.0f)),
	cdScaled(cd*kd), uniformRadiance(cdScaled*INV_PI) {
}
	
LambertianBRDF::LambertianBRDF(float kd, const Color& cd) {
	this->kd = kd;
	this->cd = cd;
	this->cdScaled = cd*kd;
	this->uniformRadiance = this->cdScaled*INV_PI;
}

Color LambertianBRDF::GetRadiance(const IntersectionResult& IntersectionResult,
	const Vector3 &wo)  {
	return this->uniformRadiance;
}

Color LambertianBRDF::GetRho(const IntersectionResult& IntersectionResult,
	const Vector3 &wo) {
	return this->cdScaled;
}