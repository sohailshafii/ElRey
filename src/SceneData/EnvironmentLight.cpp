#include "EnvironmentLight.h"

EnvironmentLight::EnvironmentLight(bool castsShadows,
								   RandomSamplerType randomSamplerType,
								   unsigned int numRandomSamples,
								   unsigned int numRandomSets,
								   std::shared_ptr<Material> const& iMaterial) :
	Light(castsShadows) {
	sampler = SamplerCreator::CreatorSampler(randomSamplerType, numRandomSamples, numRandomSets);
	sampler->MapSamplesToHemisphere(1.0f);
	material = iMaterial;
}

EnvironmentLight::~EnvironmentLight() {
	if (sampler != nullptr) {
		delete sampler;
	}
}

// when tracing to area light, need to get position
// via sampling. same goes with normal values
Vector3 EnvironmentLight::GetDirectionFromPositionScaled(
	const IntersectionResult& intersectionRes) const {
	return Vector3();
}

void EnvironmentLight::ComputeAndStoreAreaLightInformation(
	IntersectionResult& intersectionRes) const
{
}

Color3 EnvironmentLight::GetRadiance(const IntersectionResult& intersectionRes, const Scene& scene) {
	return Color3::Black();
}

float EnvironmentLight::GeometricTerm(
	const IntersectionResult& intersectionRes)
	const {
	return 1.0f;
}
