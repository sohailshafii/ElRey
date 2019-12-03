
#include "ThinLensCamera.h"
#include "Sampling/GenericMultiSampler.h"
#include "SceneData/Scene.h"
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
	viewPlaneSampler->MapSamplesToUnitDisk();
}

// TODO fix
 void ThinLensCamera::CastIntoScene(unsigned char* pixels, unsigned int bytesPerPixel, const Scene* scene) const {
	unsigned int numSamples = viewPlaneSampler->GetNumSamples();
	Ray rayToCast;
	rayToCast.SetOrigin(eyePosition);
	unsigned int numPixels = numColumnsPixels*numRowsPixels;
	float invGamma = (1.0f/1.8f);

	float halfWidth = viewPlaneWidth * 0.5f;
	float halfHeight = viewPlaneHeight * 0.5f;
	Vector3 upVectorDisplacement = up*pixelRowHeight;

	for (unsigned int pixelIndex = 0, byteIndex = 0; pixelIndex < numPixels;
		pixelIndex++, byteIndex += bytesPerPixel) {
		float tMax = maxCastDist;
		Color accumColor = Color::Black();
		Color sampleColor = Color::Black();
		Point3& oldOrigin = gridPositions[pixelIndex];
		// TODO: fix stupid code
		for (unsigned int sampleIndex = 0; sampleIndex < numSamples; sampleIndex++) {
			/*Point2 newSample = viewPlaneSampler->GetSampleOnUnitSquare();
			Point2 newPixelPnt(pixelColWidth*((float)column - halfWidth
				+ newSample[0],
				pixelRowHeight*((float)row - halfHeight
				+ newSample[1]);
			Point2 unitDiskPoint = viewPlaneSampler->GetSampleOnUnitDisk();
			Point2 lensPoint = unitDiskPoint * lensRadius;

			rayToCast.SetOrigin(eyePosition + right * lensPoint[0] +
				up * lensPoint[1]);
			rayToCast.SetDirection(GetRayDirection(newPixelPnt, lensPoint));
			Vector3 vecToPixelCenter = newPixelPnt - eyePosition;
			vecToPixelCenter.Normalize();
			rayToCast.SetDirection(vecToPixelCenter);
			tMax = maxCastDist;
			scene->Intersect(rayToCast, sampleColor, 0.0f, tMax);
			accumColor += sampleColor;*/
		}

		accumColor /= (float)numSamples;
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
