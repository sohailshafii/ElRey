
#include "PinholeCamera.h"
#include "Sampling/GenericSampler.h"
#include "SceneData/Scene.h"

PinholeCamera::PinholeCamera(const Point3& eyePosition, const Point3& lookAtPosition,
unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
unsigned int numRandomSamples, unsigned int numRandomSets)
	: Camera(eyePosition, lookAtPosition, numColumnsPixels, numRowsPixels, viewPlaneWidth, viewPlaneHeight, up, randomSamplerType,
			 numRandomSamples, numRandomSets) {
}

 Color PinholeCamera::GetColorFromCast(unsigned int pixelRow, unsigned int pixelHeight,
 unsigned int oneDimPixelIndex, const Scene* scene) const {
	 
	 float tMax = maxCastDist;
	 Color accumColor = Color::Black();
	 Color sampleColor = Color::Black();
	 
	 Point3& oldOrigin = gridPositions[oneDimPixelIndex];
	 unsigned int numSamples = viewPlaneSampler->GetNumSamples();
	 Ray rayToCast;
	 
	 for (unsigned int sampleIndex = 0; sampleIndex < numSamples; sampleIndex++) {
		 Point2 newSample = viewPlaneSampler->GetSampleOnUnitSquare();
		 Point3 newPixelPnt = Point3(newSample[0] * pixelColWidth + oldOrigin[0],
			 -newSample[1] * pixelRowHeight + oldOrigin[1], oldOrigin[2]);
		 Vector3 vecToPixelCenter = newPixelPnt - eyePosition;
		 vecToPixelCenter.Normalize();
		 rayToCast.SetDirection(vecToPixelCenter);
		 tMax = maxCastDist;
		 scene->Intersect(rayToCast, sampleColor, 0.0f, tMax);
		 accumColor += sampleColor;
	 }

	 accumColor /= (float)numSamples;
	 return accumColor;
}
