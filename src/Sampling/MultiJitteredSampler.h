#pragma once

#include "Sampling/GenericMultiSampler.h"

class MultiJitteredSampler : public GenericMultiSampler {
public:
	MultiJitteredSampler(unsigned int numSets, unsigned int numSamples);
	~MultiJitteredSampler();

private:
	void GenerateSamples();
};
