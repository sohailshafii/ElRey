#pragma once

#include "Sampling/GenericMultiSampler.h"

class RandomSampler : public GenericMultiSampler {
public:
	RandomSampler(unsigned int numSets, unsigned int numSamples);
	~RandomSampler();

	void GenerateSamples();
};
