#include "AreaLight.h"

// TODO: need to hook to tracer somehow
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
Vector3 AreaLight::GetDirectionFromPosition(
	const IntersectionResult& intersectionRes) const {
	Point3 primitiveSample;
	primitive->SamplePrimitive(primitiveSample);
	Vector3 primitiveNormal = primitive->GetNormalAtPosition
		(primitiveSample);
	Vector3 vectorToLight = primitiveSample -
		intersectionRes.GetIntersectionPos();
	return vectorToLight;
}

void AreaLight::StoreAreaLightInformation(
	IntersectionResult& intersectionRes) const
{
	Point3 primitiveSample;
	primitive->SamplePrimitive(primitiveSample);
	Vector3 primitiveNormal = primitive->GetNormalAtPosition
	(primitiveSample);
	Vector3 vectorToLight = primitiveSample -
		intersectionRes.GetIntersectionPos();
	intersectionRes.SetIntersectionNormal(primitiveNormal);
	intersectionRes.SetVectorToLight(vectorToLight);
	intersectionRes.SetSamplePointOnLight(primitiveSample);
}

Color3 AreaLight::GetRadiance(const IntersectionResult& intersectionRes, const Scene& scene) {
	float nDotVectorToLight = intersectionRes.GetNormalVector()
		* intersectionRes.GetVectorToLight();
	if (nDotVectorToLight > 0.0f) {
		Color primitiveColor = primitive->GetMaterial()
			->GetDirectColor(intersectionRes);
		return Color3(primitiveColor[0], primitiveColor[1],
			primitiveColor[2]);
	}
	return Color3::Black();
}

float AreaLight::GeometricTerm(
	const IntersectionResult& intersectionRes)
	const {
	float nDotVectorToLight = intersectionRes.GetNormalVector()
		* intersectionRes.GetVectorToLight();
	float d2 = intersectionRes.GetIntersectionPos().
		GetDistanceSquared(intersectionRes.GetSamplePointOnLight());
	return nDotVectorToLight / d2;
}
