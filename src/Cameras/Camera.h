#pragma once

#include "Common.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"
#include "Math/Matrix.h"
#include "Materials/Color.h"

class GenericSampler;
class Scene;
class Point2;
class Ray;

// assume left-handed coordinate system, where z goes into screen
class Camera {
public:
	enum CameraType {
		Pinhole = 0,
		ThinLens,
		FishEye,
		SphericalPanoramicCamera,
		Orthographic
	};

	Camera();
	// Pinhole
	Camera(const Point3& eyePosition, const Point3& lookAtPosition,
		   unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
		   float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
		   unsigned int numRandomSamples, unsigned int numRandomSets);

	virtual ~Camera();

	virtual void CastIntoScene(unsigned char* pixels, unsigned int bytesPerPixel, const Scene* scene,
							   float frameTime) const;
	
	void Displace(const Vector3& displacementVector);
	void Rotate(const Matrix& matrix);
	// transform includes a rotation but also affects eye position
	void Transform(const Matrix& matrix);
	void TranslateAndRotate(const Vector3& translation, float rightRotationDegrees,
							float upRotationDegrees);

protected:
	void ComputeForward();
	void ComputeCoordinateRightAndUp();
	
	// some cameras require the final pixel to be multiplied by some value
	// like exposure time
	virtual float GetFinalPixelMultFact() const = 0;
	virtual void AffectFirstRay(Ray &ray, const Point2 &pixelPoint) const = 0;
	virtual float GetInvGamma() const {
		return 0.125f; // 1.0/8.0f by default
	}
	
	Point3 eyePosition;
	Point3 lookAtPosition;

	Vector3 right;
	Vector3 up;
	Vector3 forward;

	// some of these variables are redundant but
	// they are pre-computed for efficiency
	unsigned int numColumnsPixels;
	unsigned int numRowsPixels;
	unsigned int numPixels;
	float viewPlaneWidth;
	float viewPlaneHeight;
	float viewPlaneDistance;
	float pixelRowHeight;
	float pixelColWidth;

	GenericSampler* viewPlaneSampler;
	Point2* gridPositions;
};
