#include "AmbientLight.h"

AmbientLight::AmbientLight(const Color3& radiance, float radianceScale)
{
	radiancePreScaled = radiance*radianceScale;
}

AmbientLight::~AmbientLight()
{
}

Vector3 AmbientLight::GetDirectionFromPosition(const Point3& worldPosition)
{
	return Vector3::Zero();
}

Color3 AmbientLight::GetRadiance()
{
	return radiancePreScaled;
}
