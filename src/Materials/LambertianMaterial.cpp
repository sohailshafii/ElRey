#include "LambertianMaterial.h"

LambertianMaterial::LambertianMaterial(float ka, float kd, const Color& color) {
	ambientBRDF.setKd(ka);
	ambientBRDF.setCd(color);
	diffuseBRDF.setKd(kd);
	diffuseBRDF.setCd(color);
}

Color LambertianMaterial::GetAmbientColor(const IntersectionResult &intersectionResult) {
	return ambientBRDF.GetRho(intersectionResult);
}

Color LambertianMaterial::GetDirectColor(const IntersectionResult &intersectionResult) {
	return ambientBRDF.GetRadiance(intersectionResult);
}

