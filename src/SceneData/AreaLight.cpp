#include "AreaLight.h"

AreaLight::AreaLight(std::shared_ptr<Primitive> primitive) :
 Light(true), primitive(primitive) {
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

void AreaLight::StoreNormalAndLightVector(
	IntersectionResult& intersectionRes) const
{
	Point3 primitiveSample;
	primitive->SamplePrimitive(primitiveSample);
	Vector3 primitiveNormal = primitive->GetNormalAtPosition
	(primitiveSample);
	Vector3 vectorToLight = primitiveSample -
		intersectionRes.GetIntersectionPos();
	intersectionRes.SetIntersectionNormal(vectorToLight);
	intersectionRes.SetVectorToLight(vectorToLight);
}

Color3 AreaLight::GetRadiance(const IntersectionResult& intersectionRes, const Scene& scene) {
	//float nDotVectorToLight = -
	
	return Color3::White();
}
