#pragma once

#include "Materials/BRDFs/BRDF.h"
#include "Math/CommonMath.h"

class LambertianBRDF : public BRDF {
public:
	LambertianBRDF();
	LambertianBRDF(float kd, const Color& cd);

	Color GetRadiance(const IntersectionResult& IntersectionResult,
		const Vector3 &wo) override;
	Color GetRho(const IntersectionResult& IntersectionResult,
		const Vector3 &wo) override;

	void setKd(float kd) {
		this->kd = kd;
		this->cdScaled = cd*kd;
		this->uniformRadiance = this->cdScaled*INV_PI;
	}

	void setCd(const Color& cd) {
		this->cd = cd;
		this->cdScaled = cd*kd;
		this->uniformRadiance = this->cdScaled*INV_PI;
	}

private:
	float kd;
	Color cd;
	Color cdScaled;
	Color uniformRadiance;
};
