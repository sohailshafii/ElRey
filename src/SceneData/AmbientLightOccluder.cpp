#include "AmbientLightOccluder.h"
#include "Sampling/GenericSampler.h"
#include "Sampling/RandomSampler.h"
#include "Sampling/OneSampleSampler.h"
#include "Sampling/JitteredSampler.h"
#include "Sampling/NRooksSampler.h"
#include "Sampling/MultiJitteredSampler.h"
#include "Math/Point3.h"
#include "Scene.h"
#include "CommonMath.h"
#include <limits>

AmbientLightOccluder::AmbientLightOccluder(const Color3& radiance, float radianceScale, float minAmount, RandomSamplerType
randomSamplerType, unsigned int numRandomSamples, unsigned int numRandomSets) :
 Light(false) {
	 this->radiancePreScaled = radiance*radianceScale;
	 this->minRadiancePreScaled = radiancePreScaled*minAmount;
	 switch (randomSamplerType) {
		 case Jittered:
			 ambientSampler = new JitteredSampler(numRandomSets, numRandomSamples);
			 break;
		 case Random:
			 ambientSampler = new RandomSampler(numRandomSets, numRandomSamples);
			 break;
		 case NRooks:
			 ambientSampler = new NRooksSampler(numRandomSets, numRandomSamples);
			 break;
		 case MultiJittered:
			 ambientSampler = new MultiJitteredSampler(numRandomSets, numRandomSamples);
			 break;
		 default:
			 ambientSampler = new OneSampleSampler();
			 break;
	 }
	 ambientSampler->MapSamplesToHemisphere(1.0f);
}

AmbientLightOccluder::~AmbientLightOccluder() {
	if (ambientSampler != nullptr) {
		delete ambientSampler;
	}
}

Vector3 AmbientLightOccluder::GetDirectionFromPosition(
const IntersectionResult& intersectionRes) {
	Point3 sp = ambientSampler->GetSampleOnHemisphere();
	Vector3 right;
	Vector3 up = Vector3::Up();
	Vector3 forward = intersectionRes.GetNormalVector();
	CommonMath::ComputeUVWFromWandU(right, up, forward);
	return right*sp[0] + up*sp[1] + forward*sp[2];
}

Color3 AmbientLightOccluder::GetRadiance(const IntersectionResult& intersectionRes, const Scene& scene) {
	Vector3 castVec = GetDirectionFromPosition(intersectionRes);
	Point3 castPoint = intersectionRes.GetIntersectionPos()+
		castVec*SHADOW_FEELER_EPSILON;
	bool shadowFeelerIntersects = scene.ShadowFeelerIntersectsAnObject(Ray(castPoint, castVec), 0.0f, std::numeric_limits<float>::max());
	
	return shadowFeelerIntersects ? minRadiancePreScaled : radiancePreScaled;
}

