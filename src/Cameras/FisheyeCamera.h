
#include "Camera.h"
#include "Math/Point2.h"
#include "Math/Vector3.h"
#include "Math/Ray.h"
#include "CommonMath.h"
#include <cmath>

class GenericSampler;

class FisheyeCamera : public Camera {
public:
	FisheyeCamera(const Point3& eyePosition, const Point3& lookAtPosition,
		unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
		float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
		unsigned int numRandomSamples, unsigned int numRandomSets,
		float psiMax, float exposureTime);
	~FisheyeCamera();
	
protected:
	float GetFinalPixelMultFact() const override {
		return finalMultFactor;
	}
	
	void AffectFirstRay(Ray &ray, const Point2 &pixelPoint) const override {
		float normDeviceX = normFactorX * pixelPoint[0];
		float normDeviceY = normFactorY * pixelPoint[1];
		float rSquared = normDeviceX * normDeviceX + normDeviceY * normDeviceY;
		float radius = sqrt(rSquared);
		Vector3 rayDirection;

		if (rSquared <= 1.0f) {
			float psi = radius * psiMax * (float)DEG_2_RAD;
			float sinPsi = sin(psi);
			float cosPsi = cos(psi);
			float sinAlpha = normDeviceY / radius;
			float cosAlpha = normDeviceX / radius;
			rayDirection = right * sinPsi * cosAlpha +
				up * sinPsi * sinAlpha + forward * cosPsi;
		}

		ray.SetDirection(rayDirection);
	}
	
private:
	float psiMax;
	float exposureTime;
	float finalMultFactor;
	float normFactorX;
	float normFactorY;
};
