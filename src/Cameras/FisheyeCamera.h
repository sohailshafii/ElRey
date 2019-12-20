
#include "Camera.h"
#include "Math/Point2.h"
#include "Math/Vector3.h"
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
	
	Vector3 GetRayDirectionForPixelPoint(const Point2 &pixelPoint) const override {
		Point2 normalizedDeviceCoords(normFactorX * pixelPoint[0],
									 normFactorY * pixelPoint[1]);
		float rSquared = normalizedDeviceCoords[0] * normalizedDeviceCoords[0]
			+ normalizedDeviceCoords[1] * normalizedDeviceCoords[1];
		float radius = sqrt(rSquared);
		Vector3 rayDirection;

		if (rSquared <= 1.0f) {
			float psi = radius * psiMax * (float)DEG_2_RAD;
			float sinPsi = sin(psi);
			float cosPsi = cos(psi);
			float sinAlpha = normalizedDeviceCoords[1] / radius;
			float cosAlpha = normalizedDeviceCoords[0] / radius;
			rayDirection = right * sinPsi * cosAlpha +
				up * sinPsi * sinAlpha + forward * cosPsi;
		}

		return rayDirection;
	}
	
private:
	float psiMax;
	float exposureTime;
	float finalMultFactor;
	float normFactorX;
	float normFactorY;
};
