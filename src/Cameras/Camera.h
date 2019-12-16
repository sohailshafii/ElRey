#pragma once

#include "Common.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"
#include "Materials/Color.h"

class GenericSampler;
class Scene;
class Point2;

// TODO: move all camera logic into here, extra camera classes
// cause confusion

class Camera {
public:
	enum CameraType { Pinhole, ThinLens, FishEye, SphericalPanoramicCamera };

	Camera();
	// Pinhole
	Camera(const Point3& eyePosition, const Point3& lookAtPosition,
		   unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
		   float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
		   unsigned int numRandomSamples, unsigned int numRandomSets);
	// Thin lens
	/*Camera(const Point3& eyePosition, const Point3& lookAtPosition,
		   unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
		   float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
		   unsigned int numRandomSamples, unsigned int numRandomSets,
		   float lensRadius, float focalPlaneDistance, float exposureTime);
	// Fisheye
	Camera(const Point3& eyePosition, const Point3& lookAtPosition,
		   unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
		   float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
		   unsigned int numRandomSamples, unsigned int numRandomSets,
		   float psiMax, float exposureTime);
	// spherical panoramic camera
	Camera(const Point3& eyePosition, const Point3& lookAtPosition,
		   unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
		   float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
		   unsigned int numRandomSamples, unsigned int numRandomSets,
		   float psiMax, float lambdaMax, float exposureTime);*/

	virtual ~Camera();

	virtual void CastIntoScene(unsigned char* pixels, unsigned int bytesPerPixel, const Scene* scene) const = 0;

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
	Point2* gridPositions;
};
