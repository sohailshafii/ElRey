#pragma once

#include "Materials/BRDFs/BRDF.h"
#include "Math/CommonMath.h"

class GlossySpecularBRDF : public BRDF {
public:
	GlossySpecularBRDF();
	GlossySpecularBRDF(float ks, Color3 cs, float exponent);

	Color3 GetRadiance(const IntersectionResult& IntersectionResult) override;
	Color3 GetRho(const IntersectionResult& IntersectionResult) override;

	void setKs(float ks) {
		this->ks = ks;
		this->csScaled = cs*ks;
	}

	void setCs(const Color3& cs) {
		this->cs = cs;
		this->csScaled = cs*ks;
	}
	
	void setExponent(float exponent) {
		this->exponent = exponent;
	}

private:
	float ks;
	Color3 cs;
	Color3 csScaled;
	float exponent;
};

