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
	primitive->SamplePrimitiveWorld(primitiveSample);
	Vector3 lightDirection = intersectionRes.GetIntersectionPos() -
		primitiveSample;
	return lightDirection;
}

void AreaLight::ComputeAndStoreAreaLightInformation(
	IntersectionResult& intersectionRes) const {
	Point3 lightPrimitiveSample;
	primitive->SamplePrimitiveWorld(lightPrimitiveSample);
	
	auto oldIntersection = intersectionRes.GetIntersectionPos();
	// set intersection position to calculate correct sample
	// TODO: this is confusing...
	intersectionRes.SetIntersectionPosition(lightPrimitiveSample);
	Vector3 lightPrimitiveNormal = primitive->GetNormalWorld
		(intersectionRes);
	intersectionRes.SetIntersectionPosition(oldIntersection);
	
	Vector3 vectorToLight = lightPrimitiveSample -
		intersectionRes.GetIntersectionPos();
	intersectionRes.SetAreaLightNormal(lightPrimitiveNormal);
	intersectionRes.SetLightVectorScaled(vectorToLight);
	vectorToLight.Normalize();
	intersectionRes.SetLightVector(vectorToLight);
	intersectionRes.SetSamplePointOnLight(lightPrimitiveSample);
}

Color3 AreaLight::GetRadiance(const IntersectionResult& intersectionRes, const Scene& scene) {
	float nDotVectorToLight = -intersectionRes.GetAreaLightNormal()
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
	float nDotVectorToLight = -intersectionRes.GetAreaLightNormal()
		* intersectionRes.GetVectorToLight();
	float d2 = intersectionRes.GetIntersectionPos().
		GetDistanceSquared(intersectionRes.GetSamplePointOnLight());
	return nDotVectorToLight / d2;
}
