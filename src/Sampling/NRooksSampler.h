#pragma once

#include "Sampling/GenericMultiSampler.h"

class NRooksSampler : public GenericMultiSampler {
public:
	NRooksSampler(unsigned int numSets, unsigned int numSamples);
	~NRooksSampler();
	
	virtual NRooksSampler* clone() const override {
		return new NRooksSampler(*this);
	}

private:
	void GenerateSamples();
	void ShuffleSamplesCoords(bool swapX);
};
