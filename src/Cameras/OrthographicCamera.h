
#include "Camera.h"
#include "Math/Point2.h"
#include "Math/Ray.h"

class OrthographicCamera : public Camera {
public:
	OrthographicCamera(const Point3& eyePosition, const Point3& lookAtPosition,
				unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
				float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
				unsigned int numRandomSamples, unsigned int numRandomSets);

protected:
	float GetFinalPixelMultFact() const override {
		return finalMultFactor;
	}
	
	void AffectFirstRay(Ray &ray, const Point2 &pixelPoint) const override {
		Vector3 vectorToProjectionPlane = right * pixelPoint[0] + up * pixelPoint[1]
			+ forward * viewPlaneDistance;
		Point3 positionOnProjectionPlane = eyePosition + vectorToProjectionPlane;
		ray.SetOrigin(positionOnProjectionPlane);
		ray.SetDirection(forward);
	}

private:
	float finalMultFactor;
};

