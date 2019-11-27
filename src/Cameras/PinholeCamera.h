
#include "Camera.h"

class PinholeCamera : public Camera {
public:
	PinholeCamera(const Point3& eyePosition, const Point3& lookAtPosition,
		float horizontalFov, const Vector3& up, unsigned int frameBufferWidth,
		unsigned int frameBufferHeight, float viewPlaneDistance,
		RandomSamplerType randomSamplerType, unsigned int numRandomSamples,
		unsigned int numRandomSets);
	
	Vector3 GetRayDirectionToWorldPoint(unsigned int pixelRow,
		unsigned int pixelHeight) const override;

private:
	float viewPlaneDistance;
};