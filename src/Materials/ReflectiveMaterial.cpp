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
	this->reflectivity = cr*kr;
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

void ReflectiveMaterial::SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<DirectionSample> & directionSamples) const  {
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		Vector3 diffuseWi, specularWi;
		float diffusePdf, specularPdf;
		Color3 diffuseColor = diffuseBRDF.SampleF(shadingInfo, diffusePdf, diffuseWi);
		Color3 specularColor = glossySpecularBRDF.SampleF(shadingInfo, specularPdf, specularWi);
		directionSamples.push_back(DirectionSample(Color(diffuseColor[0], diffuseColor[1], diffuseColor[2], 1.0f), diffusePdf, diffuseWi));
		directionSamples.push_back(DirectionSample(Color(specularColor[0], specularColor[1], specularColor[2], 1.0f), specularPdf, specularWi));
	}

	return deadColor;
}

void ReflectiveMaterial::SetSampler(GenericSampler *sampler) {
	ambientBRDF.setSampler(sampler);
	diffuseBRDF.setSampler(sampler->clone());
	glossySpecularBRDF.setSampler(sampler->clone());
	perfectSpecularBRDF.setSampler(sampler->clone());
}

void ReflectiveMaterial::GetSecondaryVectors(Vector3 const &normal,
											 Vector3 const &incomingVecFacingAwaySurface,
											 std::vector<SecondaryVectorInfo> & secondaryVectors) const {
	Vector3 reflectedVec =
		perfectSpecularBRDF.GetReflectionVector(incomingVecFacingAwaySurface,
												normal);
	secondaryVectors.push_back(SecondaryVectorInfo(reflectedVec, reflectivity));
}
