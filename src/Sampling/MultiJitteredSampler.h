#pragma once

#include "Sampling/GenericMultiSampler.h"

class MultiJitteredSampler : public GenericMultiSampler {
public:
	MultiJitteredSampler(unsigned int numSets, unsigned int numSamples);
	~MultiJitteredSampler();
	
	virtual MultiJitteredSampler* clone() const override {
		return new MultiJitteredSampler(*this);
	}

private:
	void GenerateSamples();
};
