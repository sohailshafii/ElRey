#include "PointLight.h"

PointLight::PointLight(bool castsShadows, const Point3 &lightPosition,
	const Color3& radiance, float radianceScale) : Light(castsShadows) {
	this->lightPosition = lightPosition;
	this->radiancePreScaled = radiance*radianceScale;
}

PointLight::~PointLight() {
}

Vector3 PointLight::GetDirectionFromPositionScaled(const IntersectionResult& intersectionRes)
 const {
	Vector3 directionFromPos = (intersectionRes.intersectionPosition - lightPosition);
	directionFromPos.Normalize();
	return directionFromPos;
}

Color3 PointLight::GetRadiance(const IntersectionResult& intersectionRes, const Scene& scene) const {
	return radiancePreScaled;
}
