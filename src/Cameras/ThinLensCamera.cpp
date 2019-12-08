
#include "ThinLensCamera.h"
#include "Sampling/GenericMultiSampler.h"
#include "SceneData/Scene.h"
#include "Sampling/RandomSampler.h"
#include "Sampling/OneSampleSampler.h"
#include "Sampling/JitteredSampler.h"
#include "Sampling/NRooksSampler.h"
#include "Sampling/MultiJitteredSampler.h"
#include <stdexcept>

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

	switch (randomSamplerType) {
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

// TODO fix
 void ThinLensCamera::CastIntoScene(unsigned char* pixels, unsigned int bytesPerPixel, const Scene* scene) const {
	unsigned int numSamples = viewPlaneSampler->GetNumSamples();
	Ray rayToCast;
	rayToCast.SetOrigin(eyePosition);
	unsigned int numPixels = numColumnsPixels*numRowsPixels;
	float invGamma = (1.0f/1.8f);
	Point2 diskPoint, lensPoint;

	float finalColorMultFactor = (float)exposureTime / (float)numSamples;

	for (unsigned int pixelIndex = 0, byteIndex = 0; pixelIndex < numPixels;
		pixelIndex++, byteIndex += bytesPerPixel) {
		float tMax = maxCastDist;
		Color accumColor = Color::Black();
		Color sampleColor = Color::Black();
		const Point2& oldOrigin = gridPositions[pixelIndex];

		for (unsigned int sampleIndex = 0; sampleIndex < numSamples; sampleIndex++) {
			Point2 newSample = viewPlaneSampler->GetSampleOnUnitSquare();
			Point2 newPixelPnt = oldOrigin;
			newPixelPnt[0] += pixelColWidth * newSample[0];
			newPixelPnt[1] += pixelRowHeight * newSample[1];
			diskPoint = diskSampler->GetSampleOnUnitDisk();
			lensPoint = diskPoint * lensRadius;

			rayToCast.SetOrigin(eyePosition + right * lensPoint[0]
				+ up * lensPoint[1]);
			rayToCast.SetDirection(GetRayDirection(newPixelPnt, lensPoint));
			tMax = maxCastDist;
			scene->Intersect(rayToCast, sampleColor, 0.0f, tMax);
			accumColor += sampleColor;
		}

		accumColor *= finalColorMultFactor;

		// gamma-correct
		accumColor ^= invGamma;
		pixels[byteIndex] = (unsigned char)(accumColor[2] * 255.0f); // B
		pixels[byteIndex + 1] = (unsigned char)(accumColor[1] * 255.0f); // G
		pixels[byteIndex + 2] = (unsigned char)(accumColor[0] * 255.0f); // R
		if (bytesPerPixel == 4) {
			pixels[byteIndex + 3] = 255;
		}
	}
}
