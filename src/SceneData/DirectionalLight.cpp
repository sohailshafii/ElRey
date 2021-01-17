#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(bool castsShadows,
	const Vector3& direction, const Color3& radiance,
	float radianceScale) : Light(castsShadows) {
	this->lightDirection = direction;
	this->lightDirection.Normalize();
	this->radiancePreScaled = radiance*radianceScale;
}

DirectionalLight::~DirectionalLight() {
}

Vector3 DirectionalLight::GetDirectionFromPositionScaled(const ShadingInfo& shadingInfo) const {
	return lightDirection;
}

Color3 DirectionalLight::GetRadiance(const ShadingInfo& shadingInfo, const Scene& scene) const {
	return radiancePreScaled;
}

