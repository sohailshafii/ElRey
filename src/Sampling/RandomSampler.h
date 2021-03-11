#pragma once

#include "Sampling/GenericMultiSampler.h"

class RandomSampler : public GenericMultiSampler {
public:
	RandomSampler(unsigned int numSets, unsigned int numSamples);
	~RandomSampler();
	
	virtual RandomSampler* clone() const override {
		return new RandomSampler(*this);
	}

private:
	void GenerateSamples();
};
