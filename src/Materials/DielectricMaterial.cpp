#include "DielectricMaterial.h"
#include "Sampling/GenericSampler.h"
#include <vector>

DielectricMaterial::DielectricMaterial(float ka, float kd, float ks, float exponent,
									   const Color3& color, const Color3& ksColor,
									   float etaIn, float etaOut,
									   Color3 const & cfIn, Color3 const & cfOut) :
		PhongMaterial(ka, kd, ks, exponent, color, ksColor) {
	fresnelBrdf.SetEtaIn(etaIn);
	fresnelBrdf.SetEtaOut(etaOut);
	fresnelBrdf.SetCs(ksColor);
	fresnelBrdf.SetExponent(exponent);
			
	fresnelBtdf.SetColor(ksColor);
	fresnelBtdf.SetEta(etaIn);
	this->etaOut = etaOut;
	this->cfIn = cfIn;
	this->cfOut = cfOut;
}

void DielectricMaterial::SetSampler(GenericSampler *sampler) {
	PhongMaterial::SetSampler(sampler);
	fresnelBrdf.SetSampler(sampler->clone());
	fresnelBtdf.SetSampler(sampler->clone());
}

void DielectricMaterial::GetSecondaryVectors(ShadingInfo const & shadingInfo,
											  std::vector<SecondaryVectorInfo> & secondaryVectors) const {
	
	bool allowsTransmission = fresnelBtdf.AllowsTransmission(shadingInfo);
	auto const & shadingNormal = shadingInfo.normalVector;
	float ndotwi = shadingNormal*shadingInfo.wi;
	Vector3 reflectedVec;
	float fresnelBrdfPDF;
	Color3 brdfColor = fresnelBrdf.SampleF(shadingInfo, fresnelBrdfPDF, reflectedVec);
	
	// reflection only case
	if (!allowsTransmission) {
		// TODO: fix reflectivity
		secondaryVectors.push_back(SecondaryVectorInfo(reflectedVec, 1.0f,
													   Color(brdfColor[0],
															 brdfColor[1],
															 brdfColor[2], 1.0f),
													   ndotwi < 0.0f ? cfIn : cfOut,
													   true));
	}
	else {
		float btdfPdf;
		Vector3 transmittedVec;
		float transmission;
		Color btdfColor = fresnelBtdf.SampleF(shadingInfo, btdfPdf, transmittedVec, transmission);
		float ndotwt = shadingNormal * transmittedVec;
		secondaryVectors.push_back(SecondaryVectorInfo(reflectedVec, 1.0f - transmission,
													   Color(brdfColor[0],
															 brdfColor[1],
															 brdfColor[2], 1.0f),
													   ndotwt < 0.0f ? cfIn : cfOut,
													   true));
		
		secondaryVectors.push_back(SecondaryVectorInfo(transmittedVec, transmission,
													   Color(btdfColor[0],
															 btdfColor[1],
															 btdfColor[2], 1.0f),
													   ndotwt < 0.0f ? cfOut : cfIn,
													   true));
	}
}
