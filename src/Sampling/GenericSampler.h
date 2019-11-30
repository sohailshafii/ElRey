#pragma once

#include "Math/Point2.h"

class GenericSampler {
public:
	GenericSampler();
	GenericSampler(unsigned int numSamples);
	GenericSampler(unsigned int numSets, unsigned int numSamples);

	virtual ~GenericSampler();

	virtual Point2 GetSampleOnUnitSquare() = 0;
	virtual Point2 GetSampleOnUnitDisk() = 0;

protected:
	unsigned int numSets;
	unsigned int numSamples;
	Point2* samples;

private:
	void AllocateGenericSamples(unsigned int numSets, unsigned int numSamples);
};
