#include "PerfectSpecularBRDF.h"
#include "Sampling/GenericSampler.h"

PerfectSpecularBRDF::PerfectSpecularBRDF() : sampler(nullptr), ks(0), cs(Color3(0.0f, 0.0f, 0.0f)), csScaled(cs*ks), exponent(0.0f) {
}
	
PerfectSpecularBRDF::PerfectSpecularBRDF(GenericSampler *sampler, float ks, Color3 cs, float exponent) : sampler(sampler), ks(ks),
	cs(cs), csScaled(cs*ks), exponent(exponent) {
	sampler->MapSamplesToHemisphere(exponent);
}

PerfectSpecularBRDF::~PerfectSpecularBRDF() {
	if (sampler != nullptr) {
		delete sampler;
	}
}

Vector3 PerfectSpecularBRDF::GetReflectionVector(Vector3 const & wo,
												 Vector3 const & normal) const {
	float ndotwo = normal * wo;
	return (-wo + normal * ndotwo * 2.0f).Normalized();
}

Color3 PerfectSpecularBRDF::F(ShadingInfo& shadingInfo) const {
	return Color3::Black();
}

Color3 PerfectSpecularBRDF::SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &newWi) const {
	Vector3 const & normal = shadingInfo.normalVector;
	Vector3 reflectedVec = GetReflectionVector(shadingInfo.wo, normal);
	newWi = reflectedVec;
	// strongest where we reflect perfectly
	pdf = fabs(normal*reflectedVec);
	return csScaled;
}

Color3 PerfectSpecularBRDF::GetRho(const ShadingInfo& shadingInfo) const {
	return Color3::Black();
}

