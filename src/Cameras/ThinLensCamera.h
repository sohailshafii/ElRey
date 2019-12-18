
#include "Camera.h"
#include "Math/Point2.h"
#include "Math/Vector3.h"

class GenericSampler;

class ThinLensCamera : public Camera {
public:
	ThinLensCamera(const Point3& eyePosition, const Point3& lookAtPosition,
		unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
		float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
		unsigned int numRandomSamples, unsigned int numRandomSets,
		float lensRadius, float focalPlaneDistance, float exposureTime);
	~ThinLensCamera();
	
	void CastIntoScene(unsigned char* pixels, unsigned int bytesPerPixel, const Scene* scene) const override;
	
protected:
	float GetFinalPixelMultFact() const override {
		return finalMultFactor;
	}
	
	Vector3 GetRayDirectionForPixelPoint(const Point2 &pixelPoint) const override {
		return Vector3::Zero();
	}

private:
	float lensRadius;
	float focalPlaneDistance;
	float exposureTime;
	float finalMultFactor;

	GenericSampler* diskSampler;

	inline Vector3 GetRayDirection(const Point2& pixelPoint,
		const Point2& lensPoint) const;
};

inline Vector3 ThinLensCamera::GetRayDirection(const Point2& pixelPoint,
	const Point2& lensPoint) const {
	float focalRatio = focalPlaneDistance / viewPlaneDistance;
	Point2 pointOnFocalPlane(pixelPoint[0] * focalRatio,
		pixelPoint[1] * focalRatio);
	Vector3 direction = right * (pointOnFocalPlane[0] - lensPoint[0])
		+ up * (pointOnFocalPlane[1] - lensPoint[1])
		+ forward * focalPlaneDistance;
	direction.Normalize();
	return direction;
}
