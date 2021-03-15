
#include "GlossySpecularMaterial.h"
#include "Sampling/GenericSampler.h"
#include "CommonMath.h"
#include <vector>

GlossySpecularMaterial::GlossySpecularMaterial(float ka, float kd, float ks,
											   float exponent, const Color3& color,
											   const Color3& ksColor, float cr, float kr) {
	ambientBRDF.setKd(ka);
	ambientBRDF.setCd(color);
	diffuseBRDF.setKd(kd);
	diffuseBRDF.setCd(color);
	
	glossySpecularBRDF.setKs(ks);
	glossySpecularBRDF.setExponent(exponent);
	glossySpecularBRDF.setCs(ksColor);

	deadColor = Color::Black();
	this->cr = cr;
	this->kr = kr;
}

Color GlossySpecularMaterial::GetAmbientColor(const ShadingInfo& shadingInfo) const  {
	Color3 ambientColor = ambientBRDF.GetRho(shadingInfo);
	return Color(ambientColor[0], ambientColor[1], ambientColor[2], 1.0f);
}

Color GlossySpecularMaterial::GetDirectColor(ShadingInfo& shadingInfo) const  {
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		Color3 directColor = diffuseBRDF.F(shadingInfo);
		Color3 specularColor = glossySpecularBRDF.F(shadingInfo);
		Color resultColor = Color(directColor[0]+specularColor[0], directColor[1]+specularColor[1], directColor[2]+specularColor[2], 1.0f);

		return resultColor;
	}
	
	return deadColor;
}

void GlossySpecularMaterial::SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<Color>& colors, std::vector<float>& pdfs, std::vector<Vector3> &wis) const  {
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		Vector3 diffuseWi, specularWi;
		float diffusePdf, specularPdf;
		Color3 diffuseColor = diffuseBRDF.SampleF(shadingInfo, diffusePdf, diffuseWi);
		pdfs.push_back(diffusePdf);
		wis.push_back(diffuseWi);
		Color3 specularColor = glossySpecularBRDF.SampleF(shadingInfo, specularPdf, specularWi);
		pdfs.push_back(specularPdf);
		wis.push_back(specularWi);
		colors.push_back(Color(diffuseColor[0], diffuseColor[1], diffuseColor[2], 1.0f));
		colors.push_back(Color(specularColor[0], specularColor[1], specularColor[2], 1.0f));
	}
}

void GlossySpecularMaterial::SetSampler(GenericSampler *sampler) {
	ambientBRDF.setSampler(sampler);
	diffuseBRDF.setSampler(sampler->clone());
	glossySpecularBRDF.setSampler(sampler->clone());
}

Vector3 GlossySpecularMaterial::ReflectVectorOffSurface(Vector3 const &normal,
														Vector3 const &incomingVecFacingAwaySurface) const {
	float rDotIncomingNormal;
	return glossySpecularBRDF.GetReflectionVectorSampled(incomingVecFacingAwaySurface, normal, rDotIncomingNormal);
}
