#include "AreaLight.h"

AreaLight::AreaLight(bool castsShadows, std::string primitiveName) :
	Light(castsShadows), primitiveName(primitiveName) {
		primitive = nullptr;
}

AreaLight::AreaLight(bool castsShadows,
	Primitive* iPrimitive) :
	Light(castsShadows), primitive(iPrimitive) {
}

AreaLight::~AreaLight() {
}

// when tracing to area light, need to get position
// via sampling. same goes with normal values
Vector3 AreaLight::GetDirectionFromPositionScaled(
	const IntersectionResult& intersectionRes) const {
	Point3 primitiveSample;
	primitive->SamplePrimitive(primitiveSample, intersectionRes);
	Vector3 lightDirection = intersectionRes.intersectionPosition -
		primitiveSample;
	return lightDirection;
}

void AreaLight::ComputeAndStoreAreaLightInformation(
	IntersectionResult& intersectionRes,
	ParamsForNormal const &paramsForNormal) const {
	Point3 lightPrimitiveSample;
	primitive->SamplePrimitive(lightPrimitiveSample, intersectionRes);
	ParamsForNormal newParams = paramsForNormal;
	newParams.intersectionPosPrimSpace = lightPrimitiveSample;
	Vector3 lightPrimitiveNormal = primitive->GetNormal(newParams);
	
	Vector3 vectorToLight = lightPrimitiveSample -
		intersectionRes.intersectionPosition;
	intersectionRes.areaLightNormalVector = lightPrimitiveNormal;
	intersectionRes.vectorToLightScaled = vectorToLight;
	vectorToLight.Normalize();
	intersectionRes.vectorToLight = vectorToLight;
	intersectionRes.samplePointOnLight = lightPrimitiveSample;
}

Color3 AreaLight::GetRadiance(const IntersectionResult& intersectionRes,
							  const Scene& scene) const {
	float nDotVectorToLight = -intersectionRes.areaLightNormalVector
		* intersectionRes.vectorToLight;
	if (nDotVectorToLight > 0.0f) {
		Color primitiveColor = primitive->GetMaterial(intersectionRes)
			->GetDirectColor(intersectionRes);
		return Color3(primitiveColor[0], primitiveColor[1],
			primitiveColor[2]);
	}
	return Color3::Black();
}

float AreaLight::GeometricTerm(
	const IntersectionResult& intersectionRes)
	const {
	float nDotVectorToLight = -intersectionRes.areaLightNormalVector
		* intersectionRes.vectorToLight;
	float d2 = intersectionRes.intersectionPosition.
		GetDistanceSquared(intersectionRes.samplePointOnLight);
	return nDotVectorToLight / d2;
}
