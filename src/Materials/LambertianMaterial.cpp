#include "LambertianMaterial.h"
#include "Sampling/GenericSampler.h"
#include <vector>

LambertianMaterial::LambertianMaterial(float ka, float kd,
									   std::shared_ptr<AbstractTexture> const & color) {
	ambientBRDF.setKd(ka);
	ambientBRDF.setCd(color);
	diffuseBRDF.setKd(kd);
	diffuseBRDF.setCd(color);

	deadColor = Color3::Black();
}

Color3 LambertianMaterial::GetAmbientColor(const ShadingInfo& shadingInfo) const {
	return ambientBRDF.GetRho(shadingInfo);
}

Color3 LambertianMaterial::GetDirectColor(ShadingInfo const & shadingInfo) const {
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		return diffuseBRDF.F(shadingInfo);
	}
	return deadColor;
}

void LambertianMaterial::SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<DirectionSample> & directionSamples) const {
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		Vector3 diffuseWi;
		float diffusePdf;
		Color3 diffuseColor = diffuseBRDF.SampleF(shadingInfo, diffusePdf, diffuseWi);
		directionSamples.push_back(DirectionSample(diffuseColor, diffusePdf,
												   diffuseWi));
	}
}

void LambertianMaterial::SetSampler(GenericSampler *sampler) {
	ambientBRDF.setSampler(sampler);
	diffuseBRDF.setSampler(sampler->clone());
}
