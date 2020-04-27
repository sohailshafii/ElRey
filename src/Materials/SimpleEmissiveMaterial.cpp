#include "SimpleEmissiveMaterial.h"

SimpleEmissiveMaterial::SimpleEmissiveMaterial(float ka, float kd, const Color3& color) {
	ambientColor = Color(ka * color[0], ka * color[1], ka * color[2], 1.0f);
	directColor = Color(kd * color[0], kd * color[1], kd * color[2], 1.0f);
}

Color SimpleEmissiveMaterial::GetAmbientColor(const IntersectionResult &intersectionResult) {
	return ambientColor;
}

Color SimpleEmissiveMaterial::GetDirectColor(const IntersectionResult &intersectionResult) {
	return directColor;
}

