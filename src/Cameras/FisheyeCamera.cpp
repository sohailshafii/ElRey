#include "FisheyeCamera.h"
#include "Sampling/GenericSampler.h"
#include "Math/Ray.h"
#include "SceneData/Scene.h"

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
}

FisheyeCamera::~FisheyeCamera() {
}

// TODO: finish
void FisheyeCamera::CastIntoScene(unsigned char* pixels, unsigned int bytesPerPixel,
	const Scene* scene) const {
	unsigned int numSamples = viewPlaneSampler->GetNumSamples();
	Ray rayToCast;
	rayToCast.SetOrigin(eyePosition);
	unsigned int numPixels = numColumnsPixels * numRowsPixels;
	float invGamma = (1.0f / 1.8f);

	float finalColorMultFactor = (float)exposureTime / (float)numSamples;
	Point2 normalizedDeviceCoords;
	float normFactorX = 2.0f / (pixelColWidth * (float)numColumnsPixels);
	float normFactorY = 2.0f / (pixelRowHeight * (float)numRowsPixels);

	for (unsigned int pixelIndex = 0, byteIndex = 0; pixelIndex < numPixels;
		pixelIndex++, byteIndex += bytesPerPixel) {
		float tMax = maxCastDist;
		Color accumColor = Color::Black();
		Color sampleColor = Color::Black();
		const Point2& oldOrigin = gridPositions[pixelIndex];
		for (unsigned int sampleIndex = 0; sampleIndex < numSamples; sampleIndex++) {
			sampleColor = Color::Black();
			Point2 newSample = viewPlaneSampler->GetSampleOnUnitSquare();
			Point2 newPixelPnt = oldOrigin;
			newPixelPnt[0] += pixelColWidth * newSample[0];
			newPixelPnt[1] += pixelRowHeight * newSample[1];
			normalizedDeviceCoords[0] = normFactorX * newPixelPnt[0];
			normalizedDeviceCoords[1] = normFactorY * newPixelPnt[1];
			float rSquared = normalizedDeviceCoords[0] * normalizedDeviceCoords[0]
				+ normalizedDeviceCoords[1] * normalizedDeviceCoords[1];
			float radius = sqrt(rSquared);
			rayToCast.SetDirection(GetRayDirection(normalizedDeviceCoords,
				radius, rSquared));
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