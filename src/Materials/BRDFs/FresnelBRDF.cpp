#include "FresnelBRDF.h"
#include "Math/CommonMath.h"

FresnelBRDF::FresnelBRDF() : sampler(nullptr), ks(0), cs(Color3(0.0f, 0.0f, 0.0f)), csScaled(cs*ks), exponent(0.0f) {
}
	
FresnelBRDF::FresnelBRDF(GenericSampler *sampler, float ks, Color3 cs, float exponent,
						 float eta, float etaOut) : sampler(sampler), ks(ks),
	cs(cs), csScaled(cs*ks), exponent(exponent), eta(eta), etaOut(etaOut) {
	sampler->MapSamplesToHemisphere(exponent);
}

FresnelBRDF::~FresnelBRDF() {
	if (sampler != nullptr) {
		delete sampler;
	}
}

void FresnelBRDF::setSampler(GenericSampler *sampler) {
	if (this->sampler != nullptr) {
		delete sampler;
	}
	this->sampler = sampler;
	this->sampler->MapSamplesToHemisphere(exponent);
}


Vector3 FresnelBRDF::GetReflectionVector(Vector3 const & wo,
										 Vector3 const & normal) const {
	float ndotwo = normal * wo;
	return (-wo + normal * ndotwo * 2.0f).Normalized();
}

Color3 FresnelBRDF::F(ShadingInfo const & shadingInfo) const {
	return Color3::Black();
}

Color3 FresnelBRDF::SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &newWi) const {
	Vector3 normal = shadingInfo.normalVector;
	Vector3 reflectedVec = GetReflectionVector(shadingInfo.wo, normal);
	newWi = reflectedVec;
	// strongest where we reflect perfectly
	pdf = fabs(normal*reflectedVec);
	
	float kr = 0.0f, kt = 0.0f;
	float relativeEta, cosThetaI, cosThetaT;
	CommonMath::ComputeFresnelCoefficients(shadingInfo.wo, eta, etaOut, normal,
										   kr, kt, relativeEta, cosThetaI, cosThetaT);
	
	return csScaled*kr;
}

Color3 FresnelBRDF::GetRho(const ShadingInfo& shadingInfo) const {
	return Color3::Black();
}

float FresnelBRDF::ComputeFresnelCoeff(ShadingInfo const & shadingInfo) {
	return 1.0; // TODO
}


