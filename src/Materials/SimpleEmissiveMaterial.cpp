#include "SimpleEmissiveMaterial.h"
#include "Sampling/GenericSampler.h"

SimpleEmissiveMaterial::SimpleEmissiveMaterial(float ka, float kd, const Color3& color) {
	ambientColor = Color(ka * color[0], ka * color[1], ka * color[2], 1.0f);
	directColor = Color(kd * color[0], kd * color[1], kd * color[2], 1.0f);

	deadColor = Color::Black();
}

Color SimpleEmissiveMaterial::GetAmbientColor(const ShadingInfo &shadingInfo) const  {
	return ambientColor;
}

Color SimpleEmissiveMaterial::GetDirectColor(ShadingInfo &shadingInfo) const  {
	return directColor;
}

Color SimpleEmissiveMaterial::GetColorForAreaLight(ShadingInfo &shadingInfo) const {
	// only emit color if on the "positive" side of material
	if (shadingInfo.normalVector * shadingInfo.wo > 0.0) {
		return directColor;
	}
	return deadColor;
}

void SimpleEmissiveMaterial::SetSampler(class GenericSampler *sampler) {
	// nothing for this material, it's too simple
}

