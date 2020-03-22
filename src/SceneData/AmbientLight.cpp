#include "AmbientLight.h"

AmbientLight::AmbientLight(const Color3& radiance, float radianceScale) :
 Light(false) {
	radiancePreScaled = radiance*radianceScale;
}

AmbientLight::~AmbientLight() {
}

Vector3 AmbientLight::GetDirectionFromPosition(const IntersectionResult& intersectionRes) {
	return Vector3::Zero();
}

Color3 AmbientLight::GetRadiance(const IntersectionResult& intersectionRes, const Scene& scene) {
	return radiancePreScaled;
}
