#include "PointLight.h"

PointLight::PointLight(bool castsShadows, const Point3 &lightPosition,
	const Color3& radiance, float radianceScale) : Light(castsShadows) {
	this->lightPosition = lightPosition;
	this->radiancePreScaled = radiance*radianceScale;
}

PointLight::~PointLight() {
}

Vector3 PointLight::GetDirectionFromPositionScaled(const ShadingInfo& shadingInfo)
 const {
	Vector3 directionFromPos = (shadingInfo.intersectionPosition - lightPosition);
	directionFromPos.Normalize();
	return directionFromPos;
}

Color3 PointLight::GetRadiance(const ShadingInfo& shadingInfo, const Scene& scene) const {
	return radiancePreScaled;
}
