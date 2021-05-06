#include "SimpleEmissiveMaterial.h"
#include "Sampling/GenericSampler.h"
#include <vector>

SimpleEmissiveMaterial::SimpleEmissiveMaterial(float ka, float kd, const Color3& color) {
	ambientColor = color * ka;
	directColor = color * kd;

	deadColor = Color3::Black();
}

Color3 SimpleEmissiveMaterial::GetAmbientColor(const ShadingInfo &shadingInfo) const  {
	return ambientColor;
}

Color3 SimpleEmissiveMaterial::GetDirectColor(ShadingInfo const &shadingInfo) const  {
	// only emit color if on the "positive" side of material
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		return directColor;
	}
	return deadColor;
}

void SimpleEmissiveMaterial::SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<DirectionSample> & directionSamples) const {
	// only emit color if on the "positive" side of material
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		// doesn't reflect; don't add valid wi vector
		directionSamples.push_back(DirectionSample(directColor, 1.0f, Vector3::Zero()));
	}
}

void SimpleEmissiveMaterial::SetSampler(class GenericSampler *sampler) {
	// nothing for this material, it's too simple
}

