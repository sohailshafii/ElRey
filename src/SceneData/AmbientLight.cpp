#include "AmbientLight.h"

AmbientLight::AmbientLight(const Color3& radiance, float radianceScale) :
	Light(false) {
	radiancePreScaled = radiance*radianceScale;
}

AmbientLight::~AmbientLight() {
}

Vector3 AmbientLight::GetDirectionFromPositionScaled(const ShadingInfo &shadingInfo) const {
	return Vector3::Zero();
}

Color3 AmbientLight::GetRadiance(ShadingInfo &shadingInfo,
								 const Scene& scene) const {
	return radiancePreScaled;
}
