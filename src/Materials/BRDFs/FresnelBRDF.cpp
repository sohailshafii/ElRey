#include "FresnelBRDF.h"
#include "Math/CommonMath.h"
#include "Materials/Texturing/AbstractTexture.h"

FresnelBRDF::FresnelBRDF() : sampler(nullptr), cs(nullptr), exponent(0.0f) {
}
	
FresnelBRDF::FresnelBRDF(GenericSampler *sampler,
						 std::shared_ptr<AbstractTexture> const & cs, float exponent,
						 float eta, float etaOut) : sampler(sampler),
	cs(cs), exponent(exponent), eta(eta), etaOut(etaOut) {
	sampler->MapSamplesToHemisphere(exponent);
}

FresnelBRDF::~FresnelBRDF() {
	if (sampler != nullptr) {
		delete sampler;
	}
}

void FresnelBRDF::SetSampler(GenericSampler *sampler) {
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

// Note that fresnel coefficient is not calculated here; use btdf for that
Color3 FresnelBRDF::SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &newWi) const {
	Vector3 normal = shadingInfo.normalVector;
	Vector3 reflectedVec = GetReflectionVector(shadingInfo.wo, normal);
	newWi = reflectedVec;
	// strongest where we reflect perfectly
	pdf = fabs(normal*reflectedVec);
	
	return cs->GetColor(shadingInfo);
}

Color3 FresnelBRDF::GetRho(const ShadingInfo& shadingInfo) const {
	return Color3::Black();
}


