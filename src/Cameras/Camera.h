#pragma once

#include "Common.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"
#include "Materials/Color.h"

class GenericSampler;
class Scene;

class Camera {
public:
	Camera();
	Camera(const Point3& eyePosition, const Point3& lookAtPosition,
		unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
		float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
		unsigned int numRandomSamples, unsigned int numRandomSets);

	virtual ~Camera();

	virtual Color GetColorFromCast(unsigned int pixelRow, unsigned int pixelHeight,
								   unsigned int oneDimPixelIndex, const Scene* scene) const = 0;

protected:
	void ComputeCoordinateFrameAxes();

	Point3 eyePosition;
	Point3 lookAtPosition;

	Vector3 right;
	Vector3 up;
	Vector3 forward;

	unsigned int numColumnsPixels;
	unsigned int numRowsPixels;
	float viewPlaneWidth;
	float viewPlaneHeight;
	float viewPlaneDistance;
	float maxCastDist;
	float pixelRowHeight;
	float pixelColWidth;

	GenericSampler* viewPlaneSampler;
	Point3* gridPositions;
};
