
#include "Camera.h"
#include "Math/Point2.h"
#include "Math/Vector3.h"
#include "Math/Ray.h"
#include "Sampling/GenericMultiSampler.h"

class GenericSampler;

class ThinLensCamera : public Camera {
public:
	ThinLensCamera(const Point3& eyePosition, const Point3& lookAtPosition,
		unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
		float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
		unsigned int numRandomSamples, unsigned int numRandomSets,
		float lensRadius, float focalPlaneDistance, float exposureTime);
	~ThinLensCamera();
	
protected:
	float GetFinalPixelMultFact() const override {
		return finalMultFactor;
	}
	
	void AffectFirstRay(Ray &ray, const Point2 &pixelPoint) const override {
		Point2 diskPoint = diskSampler->GetSampleOnUnitDisk();
		Point2 lensPoint = diskPoint * lensRadius;

		ray.SetOrigin(eyePosition + right * lensPoint[0]
			+ up * lensPoint[1]);
		float focalRatio = focalPlaneDistance / viewPlaneDistance;
		Point2 pointOnFocalPlane(pixelPoint[0] * focalRatio,
			pixelPoint[1] * focalRatio);
		Vector3 direction = right * (pointOnFocalPlane[0] - lensPoint[0])
			+ up * (pointOnFocalPlane[1] - lensPoint[1])
			+ forward * focalPlaneDistance;
		direction.Normalize();
		ray.SetDirection(direction);
	}
	
	// 1.0/1.8
	float GetInvGamma() const override {
		return 0.5556f;
	}

private:
	float lensRadius;
	float focalPlaneDistance;
	float exposureTime;
	float finalMultFactor;

	GenericSampler* diskSampler;
};
