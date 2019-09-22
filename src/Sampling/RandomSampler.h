#pragma once

#include "Sampling/GenericSampler.h"

class RandomSampler : public GenericSampler {
public:
	RandomSampler(unsigned int numSets, unsigned int numSamples);
	~RandomSampler();

	Point2 genSampleOnUnitSquare() override;
};
