
#include "PinholeCamera.h"

PinholeCamera::PinholeCamera(const Point3& eyePosition, const Point3& lookAtPosition,
float horizontalFovDegrees, float aspectRatio, const Vector3& up, RandomSamplerType randomSamplerType,
unsigned int numRandomSamples, unsigned int numRandomSets)
	: Camera(eyePosition, lookAtPosition, horizontalFovDegrees, aspectRatio, up, randomSamplerType,
			 numRandomSamples, numRandomSets) {
}

Vector3 PinholeCamera::GetRayDirectionToWorldPoint(unsigned int pixelRow,
	unsigned int pixelHeight) const {
	return Vector3::Forward(); // TODO fix
}
