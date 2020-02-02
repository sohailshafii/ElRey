#include "PhongMaterial.h"

PhongMaterial::PhongMaterial(float ka, float kd, float ks, float exponent, const Color3& color) {
	ambientBRDF.setKd(ka);
	ambientBRDF.setCd(color);
	diffuseBRDF.setKd(kd);
	diffuseBRDF.setCd(color);
	
	glossySpecularBRDF.setKs(ks);
	glossySpecularBRDF.setExponent(exponent);
	glossySpecularBRDF.setCs(color);
}

Color PhongMaterial::GetAmbientColor(const IntersectionResult &intersectionResult) {
	Color3 ambientColor = ambientBRDF.GetRho(intersectionResult);
	return Color(ambientColor[0], ambientColor[1], ambientColor[2], 1.0f);
}

Color PhongMaterial::GetDirectColor(const IntersectionResult &intersectionResult) {
	Color3 directColor =  diffuseBRDF.GetRadiance(intersectionResult);
	Color3 specularColor = glossySpecularBRDF.GetRadiance(intersectionResult);
	return Color(directColor[0]+specularColor[0], directColor[1]+specularColor[1], directColor[2]+specularColor[2], 1.0f);
}
