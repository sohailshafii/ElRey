#pragma once

#include "Math/Point2.h"

class GenericSampler {
public:
	GenericSampler();
	GenericSampler(unsigned int numSets, unsigned int numSamples);

	virtual ~GenericSampler();

	virtual Point2 GenSampleOnUnitSquare() = 0;
	Point2 SampleUnitSphere();

protected:
	unsigned int numSets;
	unsigned int numSamples;
	// count is the number of sample points used
	unsigned long count;
	unsigned int jump;

	Point2* samples;
	unsigned int* shuffledIndices;

	void CreateShuffledIndices();
};
