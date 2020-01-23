#include "LambertianMaterial.h"

LambertianMaterial::LambertianMaterial(float ka, float kd, const Color3& color) {
	ambientBRDF.setKd(ka);
	ambientBRDF.setCd(color);
	diffuseBRDF.setKd(kd);
	diffuseBRDF.setCd(color);
}

Color LambertianMaterial::GetAmbientColor(const IntersectionResult &intersectionResult) {
	Color3 ambientColor = ambientBRDF.GetRho(intersectionResult);
	return Color(ambientColor[0], ambientColor[1], ambientColor[2], 1.0f);
}

Color LambertianMaterial::GetDirectColor(const IntersectionResult &intersectionResult) {
	Color3 directColor =  diffuseBRDF.GetRadiance(intersectionResult);
	return Color(directColor[0], directColor[1], directColor[2], 1.0f);
}

