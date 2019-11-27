
#include "Camera.h"

class PinholeCamera : public Camera {
public:
	PinholeCamera(const Point3& eyePosition, const Point3& lookAtPosition,
		float horizontalFov, const Vector3& up, float viewPlaneDistance,
		RandomSamplerType randomSamplerType, unsigned int numRandomSamples,
		unsigned int numRandomSets);
	
	Vector3 GetRayDirectionToWorldPoint(const Point3& point)
		const override;

private:
	float viewPlaneDistance;
};