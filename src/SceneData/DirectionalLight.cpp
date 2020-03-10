#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(bool castsShadows, const Vector3& direction, const Color3& radiance, float radianceScale) : Light(castsShadows)
{
	this->lightDirection = direction;
	this->radiancePreScaled = radiance*radianceScale;
}

DirectionalLight::~DirectionalLight()
{
}

Vector3 DirectionalLight::GetDirectionFromPosition(const Point3& worldPosition)
{
	return lightDirection;
}

Color3 DirectionalLight::GetRadiance()
{
	return radiancePreScaled;
}

