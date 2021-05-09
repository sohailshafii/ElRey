#include "SimpleEmissiveMaterial.h"
#include "Sampling/GenericSampler.h"
#include "Materials/Texturing/AbstractTexture.h"
#include <vector>

SimpleEmissiveMaterial::SimpleEmissiveMaterial(float ka, float kd,
											   std::shared_ptr<AbstractTexture> const & color) {
	this->ka = ka;
	this->kd = kd;
	this->color = color;

	deadColor = Color3::Black();
}

Color3 SimpleEmissiveMaterial::GetAmbientColor(const ShadingInfo &shadingInfo) const  {
	return color->GetColor(shadingInfo)*ka;
}

Color3 SimpleEmissiveMaterial::GetDirectColor(ShadingInfo const &shadingInfo) const  {
	// only emit color if on the "positive" side of material
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		return color->GetColor(shadingInfo)*kd;
	}
	return deadColor;
}

void SimpleEmissiveMaterial::SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<DirectionSample> & directionSamples) const {
	// only emit color if on the "positive" side of material
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		// doesn't reflect; don't add valid wi vector
		Color3 directColor = color->GetColor(shadingInfo)*kd;
		directionSamples.push_back(DirectionSample(directColor, 1.0f, Vector3::Zero()));
	}
}

void SimpleEmissiveMaterial::SetSampler(class GenericSampler *sampler) {
	// nothing for this material, it's too simple
}

