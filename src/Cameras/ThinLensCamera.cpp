
#include "ThinLensCamera.h"
#include "SceneData/Scene.h"
#include "Sampling/RandomSampler.h"
#include "Sampling/RegularSampler.h"
#include "Sampling/OneSampleSampler.h"
#include "Sampling/JitteredSampler.h"
#include "Sampling/NRooksSampler.h"
#include "Sampling/MultiJitteredSampler.h"
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

	switch (randomSamplerType) {
		case Regular:
			diskSampler = new RegularSampler(numRandomSets,
												  numRandomSamples);
			break;
		case Jittered:
			diskSampler = new JitteredSampler(numRandomSets, numRandomSamples);
			break;
		case Random:
			diskSampler = new RandomSampler(numRandomSets, numRandomSamples);
			break;
		case NRooks:
			diskSampler = new NRooksSampler(numRandomSets, numRandomSamples);
			break;
		case MultiJittered:
			diskSampler = new MultiJitteredSampler(numRandomSets, numRandomSamples);
			break;
		default:
			diskSampler = new OneSampleSampler();
			break;
	}
	diskSampler->MapSamplesToUnitDisk();
}

ThinLensCamera::~ThinLensCamera() {
	if (diskSampler != nullptr) {
		delete diskSampler;
	}
}
