#include "AmbientLightOccluder.h"
#include "Sampling/GenericSampler.h"
#include "Sampling/RandomSampler.h"
#include "Sampling/OneSampleSampler.h"
#include "Sampling/JitteredSampler.h"
#include "Sampling/NRooksSampler.h"
#include "Sampling/MultiJitteredSampler.h"
#include "CommonMath.h"

AmbientLightOccluder::AmbientLightOccluder(const Color3& radiance, float radianceScale, float minAmount, RandomSamplerType
randomSamplerType, unsigned int numRandomSamples, unsigned int numRandomSets) :
 Light(false) {
	 radiancePreScaled = radiance*radianceScale;
	 this->minAmount = minAmount;
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
	//Vector3 right; Vector3 up; Vector3 forward = 
	//CommonMath::ComputeUVWFromWandU(<#Vector3 &right#>, <#Vector3 &up#>, <#Vector3 &forward#>)
	return Vector3::Zero();
}

Color3 AmbientLightOccluder::GetRadiance() {
	return radiancePreScaled;
}

