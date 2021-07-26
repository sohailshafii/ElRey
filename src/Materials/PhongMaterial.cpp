#include "PhongMaterial.h"
#include "Sampling/GenericSampler.h"
#include <vector>

PhongMaterial::PhongMaterial(float ka, float kd, float ks, float exponent,
							 std::shared_ptr<AbstractTexture> const & color,
							 std::shared_ptr<AbstractTexture> const & ksColor) {
	ambientBRDF.setKd(ka);
	ambientBRDF.setCd(color);
	diffuseBRDF.setKd(kd);
	diffuseBRDF.setCd(color);
	
	glossySpecularBRDF.setKs(ks);
	glossySpecularBRDF.setExponent(exponent);
	glossySpecularBRDF.setCs(ksColor);

	deadColor = Color3::Black();
}

Color3 PhongMaterial::GetAmbientColor(const ShadingInfo& shadingInfo) const  {
	return ambientBRDF.GetRho(shadingInfo);
}

Color3 PhongMaterial::GetDirectColor(ShadingInfo const & shadingInfo) const  {
	//if (shadingInfo.normalVector * shadingInfo.wo > 0.0)
	{
		Color3 directColor = diffuseBRDF.F(shadingInfo);
		Color3 specularColor = glossySpecularBRDF.F(shadingInfo);
		Color3 resultColor = directColor + specularColor;

		return resultColor;
	}
	return deadColor;
}

void PhongMaterial::SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<DirectionSample> & directionSamples) const  {
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		Vector3 diffuseWi, specularWi;
		float diffusePdf, specularPdf;
		Color3 diffuseColor = diffuseBRDF.SampleF(shadingInfo, diffusePdf, diffuseWi);
		Color3 specularColor = glossySpecularBRDF.SampleF(shadingInfo, specularPdf, specularWi);
		directionSamples.push_back(DirectionSample(diffuseColor, diffusePdf, diffuseWi));
		directionSamples.push_back(DirectionSample(specularColor, specularPdf, specularWi));
	}
}

void PhongMaterial::SetSampler(GenericSampler *sampler) {
	ambientBRDF.setSampler(sampler);
	diffuseBRDF.setSampler(sampler->clone());
	glossySpecularBRDF.setSampler(sampler->clone());
}
