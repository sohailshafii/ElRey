#include "ReflectiveMaterial.h"
#include "Sampling/GenericSampler.h"
#include <vector>

ReflectiveMaterial::ReflectiveMaterial(float ka, float kd, float ks, float exponent,
									   const Color3& color, const Color3& ksColor,
									   float cr, float kr) {
	ambientBRDF.setKd(ka);
	ambientBRDF.setCd(color);
	diffuseBRDF.setKd(kd);
	diffuseBRDF.setCd(color);
	
	glossySpecularBRDF.setKs(ks);
	glossySpecularBRDF.setExponent(exponent);
	glossySpecularBRDF.setCs(ksColor);
	
	perfectSpecularBRDF.setKs(ks);
	perfectSpecularBRDF.setExponent(exponent);
	perfectSpecularBRDF.setCs(ksColor);

	deadColor = Color::Black();
	this->cr = cr;
	this->kr = kr;
}

Color ReflectiveMaterial::GetAmbientColor(const ShadingInfo& shadingInfo) const  {
	Color3 ambientColor = ambientBRDF.GetRho(shadingInfo);
	return Color(ambientColor[0], ambientColor[1], ambientColor[2], 1.0f);
}

Color ReflectiveMaterial::GetDirectColor(ShadingInfo& shadingInfo) const  {
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		Color3 directColor = diffuseBRDF.F(shadingInfo);
		Color3 specularColor = glossySpecularBRDF.F(shadingInfo);
		Color resultColor = Color(directColor[0]+specularColor[0], directColor[1]+specularColor[1], directColor[2]+specularColor[2], 1.0f);
		
		return resultColor;
	}
	
	return deadColor;
}

Color ReflectiveMaterial::SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<float>& pdfs, std::vector<Vector3> &wis) const  {
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		Vector3 diffuseWi, specularWi;
		float diffusePdf, specularPdf;
		Color3 directColor = diffuseBRDF.SampleF(shadingInfo, diffusePdf, diffuseWi);
		pdfs.push_back(diffusePdf);
		wis.push_back(diffuseWi);
		Color3 specularColor = glossySpecularBRDF.SampleF(shadingInfo, specularPdf, specularWi);
		pdfs.push_back(specularPdf);
		wis.push_back(specularWi);
		return Color(directColor[0] + specularColor[0], directColor[1] + specularColor[1],
			directColor[2] + specularColor[2], 1.0f);
	}

	return deadColor;
}

void ReflectiveMaterial::SetSampler(GenericSampler *sampler) {
	ambientBRDF.setSampler(sampler);
	diffuseBRDF.setSampler(sampler->clone());
	glossySpecularBRDF.setSampler(sampler->clone());
	perfectSpecularBRDF.setSampler(sampler->clone());
}

Vector3 ReflectiveMaterial::ReflectVectorOffSurface(Vector3 const &normal,
													Vector3 const &incomingVecFacingAwaySurface) const {
	return perfectSpecularBRDF.GetReflectionVector(incomingVecFacingAwaySurface,
												   normal);
}
