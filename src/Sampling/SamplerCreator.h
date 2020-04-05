#pragma once

#include "Sampling/GenericSampler.h"

enum RandomSamplerType { None = 0, Regular, Random, Jittered, NRooks, MultiJittered };

class SamplerCreator
{
public:
	static GenericSampler* CreatorSampler(RandomSamplerType
	randomSamplerType, unsigned int numRandomSamples, unsigned int numRandomSets);
};
