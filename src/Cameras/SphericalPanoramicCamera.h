
#include "Camera.h"
#include "Math/Point2.h"
#include "Math/Vector3.h"
#include "CommonMath.h"
#include <cmath>

class GenericSampler;

class SphericalPanoramicCamera : public Camera {
public:
	SphericalPanoramicCamera(const Point3& eyePosition, const Point3& lookAtPosition,
		unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
		float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
		unsigned int numRandomSamples, unsigned int numRandomSets,
		float psiMax, float lambdaMax, float exposureTime);
	~SphericalPanoramicCamera();
	
protected:
	float GetFinalPixelMultFact() const override {
		return exposureTime;
	}
	
	Vector3 GetRayDirectionForPixelPoint(const Point2 &pixelPoint) const override {
		Vector3 rayDirection;
		float normalizedDeviceCoordsX = normFactorX * pixelPoint[0];
		float normalizedDeviceCoordsY = normFactorY * pixelPoint[1];

		// convert NDC to angles
		float lambda = normalizedDeviceCoordsX * lambdaMaxRad;
		float psi = normalizedDeviceCoordsY * psiMaxRad;

		// compute the spherical azimuth and polar angles
		// phi and lambda are the same -- measured directly from view direction
		float phi = lambda;//(float)M_PI - lambda;
		// psi maps to phi after subtracting 90 degrees
		float theta = 0.5f * (float)M_PI - psi;

		float sinPhi = sin(phi);
		float cosPhi = cos(phi);
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);

		rayDirection = right * sinTheta * sinPhi +
			up * cosTheta + forward * sinTheta * cosPhi;

		return rayDirection;
	}
	
private:
	float psiMaxRad;
	float lambdaMaxRad;
	float exposureTime;
	float normFactorX;
	float normFactorY;
	float finalMultFactor;
};
