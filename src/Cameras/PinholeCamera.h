
#include "Camera.h"
#include "Math/Point2.h"

class PinholeCamera : public Camera {
public:
	PinholeCamera(const Point3& eyePosition, const Point3& lookAtPosition,
				unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
				float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
				unsigned int numRandomSamples, unsigned int numRandomSets);
	
	void CastIntoScene(unsigned char* pixels, unsigned int bytesPerPixel, const Scene* scene) const override;

protected:
	float GetFinalPixelMultFact() const override {
		return finalMultFactor;
	}

private:
	inline Vector3 GetRayDirection(const Point2& pixelPoint) const;

	float finalMultFactor;
};

inline Vector3 PinholeCamera::GetRayDirection(const Point2& pixelPoint) const {
	Vector3 direction = right * pixelPoint[0] + up * pixelPoint[1]
		+ forward * viewPlaneDistance;
	direction.Normalize();
	return direction;
}

