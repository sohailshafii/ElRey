#pragma once

#include "Common.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"

class GenericSampler;

class Camera {
public:
	Camera();
	Camera(const Point3& eyePosition, const Point3& lookAtPosition,
		float horizontalFov, const Vector3& up, unsigned int frameBufferWidth,
		unsigned int frameBufferHeight, RandomSamplerType randomSamplerType,
		unsigned int numRandomSamples, unsigned int numRandomSets);

	virtual ~Camera();

	virtual Vector3 GetRayDirectionToWorldPoint(unsigned int pixelRow,
		unsigned int pixelHeight) const = 0;

protected:
	void ComputeCoordinateFrameAxes();

protected:
	Point3 eyePosition;
	Point3 lookAtPosition;
	float horizontalFov;

	Vector3 right;
	Vector3 up;
	Vector3 forward;

	unsigned int width;
	unsigned int height;

	GenericSampler* viewPlaneSampler;
};
