#pragma once

#include "Sampling/GenericMultiSampler.h"

class NRooksSampler : public GenericMultiSampler {
public:
	NRooksSampler(unsigned int numSets, unsigned int numSamples);
	~NRooksSampler();

private:
	void GenerateSamples();
	void ShuffleSamplesCoords(bool swapX);
};
