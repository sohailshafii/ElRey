
#include "ThinLensCamera.h"
#include "SceneData/Scene.h"
#include <stdexcept>
#include <limits>

ThinLensCamera::ThinLensCamera(const Point3& eyePosition, const Point3& lookAtPosition,
	unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
	float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
	unsigned int numRandomSamples, unsigned int numRandomSets,
	float lensRadius, float focalPlaneDistance, float exposureTime)
	: Camera(eyePosition, lookAtPosition, numColumnsPixels, numRowsPixels, viewPlaneWidth, viewPlaneHeight, up, randomSamplerType,
		numRandomSamples, numRandomSets) {
	this->lensRadius = lensRadius;
	this->focalPlaneDistance = focalPlaneDistance;
	this->exposureTime = exposureTime;
	this->finalMultFactor = (float)exposureTime / (float)viewPlaneSampler->GetNumSamples();

	diskSampler = SamplerCreator::CreatorSampler(randomSamplerType, numRandomSamples, numRandomSets);
	diskSampler->MapSamplesToUnitDisk();
}

ThinLensCamera::~ThinLensCamera() {
	if (diskSampler != nullptr) {
		delete diskSampler;
	}
}
