#include "SimpleEmissiveMaterial.h"

SimpleEmissiveMaterial::SimpleEmissiveMaterial(float ka, float kd, const Color3& color) {
	ambientColor = Color(ka * color[0], ka * color[1], ka * color[2], 1.0f);
	directColor = Color(kd * color[0], kd * color[1], kd * color[2], 1.0f);

	deadColor = Color::Black();
}

Color SimpleEmissiveMaterial::GetAmbientColor(const IntersectionResult &intersectionResult) {
	return ambientColor;
}

Color SimpleEmissiveMaterial::GetDirectColor(const IntersectionResult &intersectionResult) {
	return directColor;
}

Color SimpleEmissiveMaterial::GetColorForAreaLight(const IntersectionResult& intersectionResult) {
	if (intersectionResult.GetNormalVector() * intersectionResult.GetIncomingDirInverse()
		> 0.0) {
		return directColor;
	}
	return deadColor;
}

