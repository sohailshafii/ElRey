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
Vector3 AreaLight::GetDirectionFromPositionScaled(const ShadingInfo& shadingInfo) const {
	Point3 primitiveSample;
	primitive->SamplePrimitive(primitiveSample, shadingInfo);
	Vector3 lightDirection = shadingInfo.intersectionPosition -
		primitiveSample;
	return lightDirection;
}

void AreaLight::ModifyShadingInfoForAreaLight(ShadingInfo& shadingInfo) const {
	Point3 lightPrimitiveSample;
	primitive->SamplePrimitive(lightPrimitiveSample, shadingInfo);
	
	// calculate normal on primitive as if casting ray toward its sample
	ShadingInfo newShadingInfo = shadingInfo;
	newShadingInfo.intersectionPosition = lightPrimitiveSample;
	Vector3 vectorToLight = lightPrimitiveSample -
		shadingInfo.intersectionPosition;
	Vector3 vectorToLighNorm = vectorToLight.Normalized();
	// eye is ray cast
	newShadingInfo.eyeDir = vectorToLighNorm;
	Vector3 lightPrimitiveNormal = primitive->GetNormal(newShadingInfo);
	
	shadingInfo.areaLightNormalVector = lightPrimitiveNormal;
	shadingInfo.wiScaled = vectorToLight;
	shadingInfo.wi = vectorToLighNorm;
	shadingInfo.samplePointOnLight = lightPrimitiveSample;
}

Color3 AreaLight::GetRadiance(ShadingInfo& shadingInfo,
							  const Scene& scene) const {
	float nDotWi = -shadingInfo.areaLightNormalVector
		* shadingInfo.wi;
	if (nDotWi > 0.0f) {
		Color primitiveColor = primitive->GetMaterial(shadingInfo)
			->GetDirectColor(shadingInfo);
		return Color3(primitiveColor[0], primitiveColor[1],
			primitiveColor[2]);
	}
	return Color3::Black();
}

float AreaLight::GeometricTerm(const ShadingInfo& shadingInfo) const {
	float nDotWi = -shadingInfo.areaLightNormalVector
		* shadingInfo.wi;
	float d2 = shadingInfo.intersectionPosition.
		GetDistanceSquared(shadingInfo.samplePointOnLight);
	return nDotWi / d2;
}
