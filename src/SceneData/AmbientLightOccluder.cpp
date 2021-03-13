#include "AmbientLightOccluder.h"
#include "Sampling/SamplerCreator.h"
#include "Math/Point3.h"
#include "Scene.h"
#include "CommonMath.h"
#include <limits>

AmbientLightOccluder::AmbientLightOccluder(const Color3& radiance,
	float radianceScale, float minAmount,
	RandomSamplerType randomSamplerType,
	unsigned int numRandomSamples, unsigned int numRandomSets) :
	Light(false) {
	 this->radiancePreScaled = radiance*radianceScale;
	 this->minRadiancePreScaled = radiancePreScaled*minAmount;
	 ambientSampler = SamplerCreator::CreatorSampler(randomSamplerType, numRandomSamples, numRandomSets);
	 ambientSampler->MapSamplesToHemisphere(1.0f);
}

AmbientLightOccluder::~AmbientLightOccluder() {
	if (ambientSampler != nullptr) {
		delete ambientSampler;
	}
}

Vector3 AmbientLightOccluder::GetDirectionFromPositionScaled(const ShadingInfo &shadingInfo) const {
	Point3 sp = ambientSampler->GetSampleOnHemisphere();
	Vector3 right;
	Vector3 up = Vector3::Up();
	Vector3 forward = shadingInfo.normalVector;
	CommonMath::ComputeUVWFromUpandForward(right, up, forward);
	return right*sp[0] + up*sp[1] + forward*sp[2];
}

Color3 AmbientLightOccluder::GetRadiance(ShadingInfo &shadingInfo,
										 const Scene& scene) const {
	Vector3 castVec = GetDirectionFromPositionScaled(shadingInfo);
	castVec.Normalize();
	Point3 castPoint = shadingInfo.intersectionPosition +
		castVec*SHADOW_FEELER_EPSILON;
	bool shadowFeelerIntersects = scene.ShadowFeelerIntersectsAnObject(Ray(castPoint, castVec), 0.0f, std::numeric_limits<float>::max());
	
	return shadowFeelerIntersects ? minRadiancePreScaled : radiancePreScaled;
}

