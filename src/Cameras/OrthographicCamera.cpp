
#include "OrthographicCamera.h"
#include "Sampling/GenericSampler.h"
#include "SceneData/Scene.h"
#include <limits>

OrthographicCamera::OrthographicCamera(const Point3& eyePosition, const Point3& lookAtPosition,
unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
unsigned int numRandomSamples, unsigned int numRandomSets)
	: Camera(eyePosition, lookAtPosition, numColumnsPixels, numRowsPixels, viewPlaneWidth, viewPlaneHeight, up, randomSamplerType,
			numRandomSamples, numRandomSets) {
	finalMultFactor = 1.0f/(float)viewPlaneSampler->GetNumSamples();
}

