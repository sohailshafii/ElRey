
#include "Sampling/SamplerCreator.h"
#include "Sampling/RegularSampler.h"
#include "Sampling/RandomSampler.h"
#include "Sampling/OneSampleSampler.h"
#include "Sampling/JitteredSampler.h"
#include "Sampling/NRooksSampler.h"
#include "Sampling/MultiJitteredSampler.h"


GenericSampler* SamplerCreator::CreatorSampler(RandomSamplerType
randomSamplerType, unsigned int numRandomSamples, unsigned int numRandomSets)
{
	GenericSampler* newSampler = nullptr;
	switch (randomSamplerType) {
		case Jittered:
			newSampler = new JitteredSampler(numRandomSets, numRandomSamples);
			break;
		case Regular:
			newSampler = new RegularSampler(numRandomSets,
												  numRandomSamples);
			break;
		case Random:
			newSampler = new RandomSampler(numRandomSets, numRandomSamples);
			break;
		case NRooks:
			newSampler = new NRooksSampler(numRandomSets, numRandomSamples);
			break;
		case MultiJittered:
			newSampler = new MultiJitteredSampler(numRandomSets, numRandomSamples);
			break;
		default:
			newSampler = new OneSampleSampler();
			break;
	}
	return newSampler;
}
