#include "LambertianMaterial.h"
#include "Sampling/GenericSampler.h"
#include <vector>

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
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		Color3 directColor =  diffuseBRDF.F(shadingInfo);
		return Color(directColor[0], directColor[1], directColor[2], 1.0f);
	}
	return deadColor;
}

void LambertianMaterial::SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<Color>& colors, std::vector<float>& pdfs, std::vector<Vector3> &wis) const {
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		Vector3 diffuseWi;
		float diffusePdf;
		Color3 diffuseColor = diffuseBRDF.SampleF(shadingInfo, diffusePdf, diffuseWi);
		pdfs.push_back(diffusePdf);
		wis.push_back(diffuseWi);
		colors.push_back(Color(diffuseColor[0], diffuseColor[1], diffuseColor[2], 1.0f));
	}
}

void LambertianMaterial::SetSampler(GenericSampler *sampler) {
	ambientBRDF.setSampler(sampler);
	diffuseBRDF.setSampler(sampler->clone());
}
