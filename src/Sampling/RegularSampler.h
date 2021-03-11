#pragma once

#include "Sampling/GenericMultiSampler.h"

class RegularSampler : public GenericMultiSampler {
public:
	RegularSampler(unsigned int numSets, unsigned int numSamples);
	~RegularSampler();
	
	virtual RegularSampler* clone() const override {
		return new RegularSampler(*this);
	}

private:
	void GenerateSamples();
};
