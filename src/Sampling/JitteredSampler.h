#pragma once

#include "Sampling/GenericMultiSampler.h"

class JitteredSampler : public GenericMultiSampler {
public:
	JitteredSampler(unsigned int numSets, unsigned int numSamples);
	~JitteredSampler();

	void GenerateSamples();
};
