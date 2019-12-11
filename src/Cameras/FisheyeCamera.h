
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

	void CastIntoScene(unsigned char* pixels, unsigned int bytesPerPixel,
		const Scene* scene) const override;

private:
	float psiMax;
	float exposureTime;

	inline Vector3 GetRayDirection(const Point2& normalizedDevCoords,
		float radius, float radiusSquared) const;
};

inline Vector3 FisheyeCamera::GetRayDirection(const Point2& normalizedDevCoords,
	float radius, float radiusSquared) const {
	Vector3 rayDirection;

	if (radiusSquared <= 1.0f) {
		float psi = radius * psiMax * (float)DEG_2_RAD;
		float sinPsi = sin(psi);
		float cosPsi = cos(psi);
		float sinAlpha = normalizedDevCoords[1] / radius;
		float cosAlpha = normalizedDevCoords[0] / radius;
		rayDirection = right * sinPsi * cosAlpha +
			up * sinPsi * sinAlpha + forward * cosPsi;
	}

	return rayDirection;
}
