
#include "PinholeCamera.h"

PinholeCamera::PinholeCamera(const Point3& eyePosition, const Point3& lookAtPosition,
	float horizontalFov, const Vector3& up, unsigned int frameBufferWidth,
	unsigned int frameBufferHeight, float viewPlaneDistance, RandomSamplerType
	randomSamplerType, unsigned int numRandomSamples, unsigned int numRandomSets)
	: Camera(eyePosition, lookAtPosition, horizontalFov, up, frameBufferWidth,
		frameBufferHeight, randomSamplerType, numRandomSamples, numRandomSets) {
	this->viewPlaneDistance = viewPlaneDistance;
}

Vector3 PinholeCamera::GetRayDirectionToWorldPoint(unsigned int pixelRow,
	unsigned int pixelHeight) const {
	return Vector3::Forward(); // TODO fix
}
