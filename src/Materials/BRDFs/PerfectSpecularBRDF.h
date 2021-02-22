
#pragma once

#include "Materials/BRDFs/BRDF.h"
#include "Math/CommonMath.h"

class PerfectSpecularBRDF : public BRDF {
public:
	PerfectSpecularBRDF();
	PerfectSpecularBRDF(float kr, Color3 cr);

	Color3 F(ShadingInfo& shadingInfo) const override;
	Color3 SampleF(ShadingInfo& shadingInfo, float& pdf) const override;
	Color3 GetRho(const ShadingInfo& shadingInfo) const override;

	void setKr(float kr) {
		this->kr = kr;
		this->crScaled = cr*kr;
	}

	void setCr(const Color3& cr) {
		this->cr = cr;
		this->crScaled = cr*kr;
	}

private:
	float kr;
	Color3 cr;
	Color3 crScaled;
};

