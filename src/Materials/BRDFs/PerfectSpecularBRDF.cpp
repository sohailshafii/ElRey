#include "PerfectSpecularBRDF.h"

PerfectSpecularBRDF::PerfectSpecularBRDF() : sampler(nullptr), ks(0), cs(nullptr), exponent(0.0f) {
}
	
PerfectSpecularBRDF::PerfectSpecularBRDF(GenericSampler *sampler, float ks,
										 std::shared_ptr<AbstractTexture> const & color,
										 float exponent) : sampler(sampler), ks(ks),
	cs(cs), exponent(exponent) {
	sampler->MapSamplesToHemisphere(exponent);
}

PerfectSpecularBRDF::~PerfectSpecularBRDF() {
	if (sampler != nullptr) {
		delete sampler;
	}
}

void PerfectSpecularBRDF::setSampler(GenericSampler *sampler) {
	if (this->sampler != nullptr) {
		delete sampler;
	}
	this->sampler = sampler;
	this->sampler->MapSamplesToHemisphere(exponent);
}


Vector3 PerfectSpecularBRDF::GetReflectionVector(Vector3 const & wo,
												 Vector3 const & normal) const {
	float ndotwo = normal * wo;
	return (-wo + normal * ndotwo * 2.0f).Normalized();
}

Color3 PerfectSpecularBRDF::F(ShadingInfo const & shadingInfo) const {
	return Color3::Black();
}

Color3 PerfectSpecularBRDF::SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &newWi) const {
	Vector3 const & normal = shadingInfo.normalVector;
	Vector3 reflectedVec = GetReflectionVector(shadingInfo.wo, normal);
	newWi = reflectedVec;
	// strongest where we reflect perfectly
	pdf = fabs(normal*reflectedVec);
	return cs->GetColor(shadingInfo);
}

Color3 PerfectSpecularBRDF::GetRho(const ShadingInfo& shadingInfo) const {
	return Color3::Black();
}

