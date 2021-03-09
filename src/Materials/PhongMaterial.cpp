#include "PhongMaterial.h"

PhongMaterial::PhongMaterial(float ka, float kd, float ks, float exponent, const Color3& color,
const Color3& ksColor) {
	ambientBRDF.setKd(ka);
	ambientBRDF.setCd(color);
	diffuseBRDF.setKd(kd);
	diffuseBRDF.setCd(color);
	
	glossySpecularBRDF.setKs(ks);
	glossySpecularBRDF.setExponent(exponent);
	glossySpecularBRDF.setCs(ksColor);

	deadColor = Color::Black();
}

Color PhongMaterial::GetAmbientColor(const ShadingInfo& shadingInfo) const  {
	Color3 ambientColor = ambientBRDF.GetRho(shadingInfo);
	return Color(ambientColor[0], ambientColor[1], ambientColor[2], 1.0f);
}

Color PhongMaterial::GetDirectColor(ShadingInfo& shadingInfo) const  {
	Color3 directColor = diffuseBRDF.F(shadingInfo);
	Color3 specularColor = glossySpecularBRDF.F(shadingInfo);
	Color resultColor = Color(directColor[0]+specularColor[0], directColor[1]+specularColor[1], directColor[2]+specularColor[2], 1.0f);

	return resultColor;
}

Color PhongMaterial::GetColorForAreaLight(ShadingInfo& shadingInfo) const  {
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		Vector3 wiMod;
		float pdf;
		Color3 directColor = diffuseBRDF.SampleF(shadingInfo, pdf,
												wiMod);
		Color3 specularColor = glossySpecularBRDF.SampleF(shadingInfo, pdf,
														  wiMod);
		return Color(directColor[0] + specularColor[0], directColor[1] + specularColor[1],
			directColor[2] + specularColor[2], 1.0f);
	}

	return deadColor;
}
