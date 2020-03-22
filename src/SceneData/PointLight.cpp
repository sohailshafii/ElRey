#include "PointLight.h"

PointLight::PointLight(bool castsShadows, const Point3 &lightPosition,
	const Color3& radiance, float radianceScale) : Light(castsShadows)
{
	this->lightPosition = lightPosition;
	this->radiancePreScaled = radiance*radianceScale;
}

PointLight::~PointLight()
{
}

Vector3 PointLight::GetDirectionFromPosition(const IntersectionResult& intersectionRes)
{
	Vector3 directionFromPos = (intersectionRes.GetIntersectionPos() - lightPosition);
	directionFromPos.Normalize();
	return directionFromPos;
}

Color3 PointLight::GetRadiance() 
{
	return radiancePreScaled;
}
