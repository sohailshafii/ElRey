#include "PointLight.h"

PointLight::PointLight(const Point3 &lightPosition,
	const Color3& radiance, float radianceScale)
{
	this->lightPosition = lightPosition;
	this->radiancePreScaled = radiance*radianceScale;
}

PointLight::~PointLight()
{
}

Vector3 PointLight::GetDirectionFromPosition(const Point3& worldPosition) 
{
	Vector3 directionFromPos = (worldPosition - lightPosition);
	directionFromPos.Normalize();
	return directionFromPos;
}

Color3 PointLight::GetRadiance() 
{
	return radiancePreScaled;
}
