#pragma once

#include "Sampling/GenericMultiSampler.h"

class RegularSampler : public GenericMultiSampler {
public:
	RegularSampler(unsigned int numSets, unsigned int numSamples);
	~RegularSampler();

private:
	void GenerateSamples();
};
