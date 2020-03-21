#include "AmbientLightOccluder.h"

AmbientLightOccluder::AmbientLightOccluder(const Color3& radiance, float radianceScale, float minAmount) :
 Light(false)
{
	radiancePreScaled = radiance*radianceScale;
	this->minAmount = minAmount;
}

AmbientLightOccluder::~AmbientLightOccluder()
{
}

Vector3 AmbientLightOccluder::GetDirectionFromPosition(const Point3& worldPosition)
{
	return Vector3::Zero();
}

Color3 AmbientLightOccluder::GetRadiance()
{
	return radiancePreScaled;
}

