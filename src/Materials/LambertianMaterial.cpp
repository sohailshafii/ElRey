#include "LambertianMaterial.h"

LambertianMaterial::LambertianMaterial(float ka, float kd, const Color3& color) {
	ambientBRDF.setKd(ka);
	ambientBRDF.setCd(color);
	diffuseBRDF.setKd(kd);
	diffuseBRDF.setCd(color);

	deadColor = Color::Black();
}

Color LambertianMaterial::GetAmbientColor(const ShadingInfo& shadingInfo) const {
	Color3 ambientColor = ambientBRDF.GetRho(shadingInfo);
	return Color(ambientColor[0], ambientColor[1], ambientColor[2], 1.0f);
}

Color LambertianMaterial::GetDirectColor(ShadingInfo& shadingInfo) const {
	Color3 directColor =  diffuseBRDF.F(shadingInfo);
	return Color(directColor[0], directColor[1], directColor[2], 1.0f);
}

Color LambertianMaterial::GetColorForAreaLight(ShadingInfo& shadingInfo) const {
	if (shadingInfo.normalVector * shadingInfo.incomingDirInverse
		> 0.0) {
		Color3 directColor = diffuseBRDF.F(shadingInfo);
		return Color(directColor[0], directColor[1], directColor[2], 1.0f);
	}
	return deadColor;
}
