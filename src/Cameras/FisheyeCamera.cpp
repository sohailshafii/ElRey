#include "FisheyeCamera.h"
#include "Sampling/GenericSampler.h"
#include "Math/Ray.h"
#include "SceneData/Scene.h"
#include <limits>

FisheyeCamera::FisheyeCamera(const Point3& eyePosition, const Point3& lookAtPosition,
	unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
	float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
	unsigned int numRandomSamples, unsigned int numRandomSets,
	float psiMax, float exposureTime)
	: Camera(eyePosition, lookAtPosition, numColumnsPixels, numRowsPixels,
		viewPlaneWidth, viewPlaneHeight, up, randomSamplerType,
		numRandomSamples, numRandomSets) {
	this->psiMax = psiMax;
	this->exposureTime = exposureTime;
	finalMultFactor = exposureTime/(float)viewPlaneSampler->GetNumSamples();
	normFactorX = 2.0f / (pixelColWidth * (float)numColumnsPixels);
	normFactorY = 2.0f / (pixelRowHeight * (float)numRowsPixels);
}

FisheyeCamera::~FisheyeCamera() {
}
