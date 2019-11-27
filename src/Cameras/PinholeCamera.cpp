
#include "PinholeCamera.h"

PinholeCamera::PinholeCamera(const Point3& eyePosition, const Point3& lookAtPosition,
	float horizontalFov, const Vector3& up, float viewPlaneDistance, RandomSamplerType
	randomSamplerType, unsigned int numRandomSamples, unsigned int numRandomSets)
	: Camera(eyePosition, lookAtPosition, horizontalFov, up, randomSamplerType,
		numRandomSamples, numRandomSets) {
	this->viewPlaneDistance = viewPlaneDistance;
}

Vector3 PinholeCamera::GetRayDirectionToWorldPoint(const Point3& point) const {
	return Vector3::Forward(); // TODO fix
}
