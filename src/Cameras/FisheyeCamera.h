
#include "Camera.h"
#include "Math/Point2.h"
#include "Math/Vector3.h"

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

	inline Vector3 GetRayDirection(float s, float &r) const;
};

inline Vector3 FisheyeCamera::GetRayDirection(float s, float& r) const {
	return Vector3::Zero();
}
