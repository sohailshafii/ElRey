
#include "Camera.h"

class PinholeCamera : public Camera {
public:
	PinholeCamera(const Point3& eyePosition, const Point3& lookAtPosition,
				  unsigned int numColumnsPixels, unsigned int numRowsPixels, float viewPlaneWidth,
				  float viewPlaneHeight, const Vector3& up, RandomSamplerType randomSamplerType,
				  unsigned int numRandomSamples, unsigned int numRandomSets);
	
	Color GetColorFromCast(unsigned int pixelRow, unsigned int pixelHeight,
						   unsigned int oneDimPixelIndex, const Scene* scene) const override;

private:
};
