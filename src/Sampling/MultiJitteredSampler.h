#pragma once

#include "Sampling/GenericMultiSampler.h"
#include <iostream>

class MultiJitteredSampler : public GenericMultiSampler {
public:
	MultiJitteredSampler(unsigned int numSets, unsigned int numSamples);
	~MultiJitteredSampler();
	
	virtual MultiJitteredSampler* clone() const override {
		std::cout << "Hello multijittered\n";
		return new MultiJitteredSampler(*this);
	}

private:
	void GenerateSamples();
};
