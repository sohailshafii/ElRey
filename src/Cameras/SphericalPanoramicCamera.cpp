#include "SphericalPanoramicCamera.h"
#include "Sampling/GenericSampler.h"
#include "SceneData/Scene.h"
#include <limits>

SphericalPanoramicCamera::SphericalPanoramicCamera(const Point3& eyePosition, const Point3& lookAtPosition,
	unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
	float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
	unsigned int numRandomSamples, unsigned int numRandomSets,
	float psiMax, float lambdaMax, float exposureTime)
	: Camera(eyePosition, lookAtPosition, numColumnsPixels, numRowsPixels,
		viewPlaneWidth, viewPlaneHeight, up, randomSamplerType,
		numRandomSamples, numRandomSets) {
	this->psiMaxRad = psiMax * (float)DEG_2_RAD;
	this->lambdaMaxRad = lambdaMax * (float)DEG_2_RAD;
	this->exposureTime = exposureTime;
	this->normFactorX = 2.0f / (pixelColWidth * (float)numColumnsPixels);
	this->normFactorY = 2.0f / (pixelRowHeight * (float)numRowsPixels);
	this->finalMultFactor = (float)exposureTime / (float)viewPlaneSampler->GetNumSamples();
}

SphericalPanoramicCamera::~SphericalPanoramicCamera() {
}
