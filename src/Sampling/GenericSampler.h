#pragma once

#include "Math/Point2.h"

class GenericSampler {
public:
	GenericSampler() {
		this->numSets = 0;
		this->numSamples = 0;
		this->samples = nullptr;
	}

	GenericSampler(unsigned int numSets, unsigned int numSamples) {
		this->numSets = numSets;
		this->numSamples = numSamples;
		this->samples = new Point2[numSets * numSamples];
	}

	virtual ~GenericSampler() {
		if (samples != nullptr) {
			delete[] samples;
		}
	}

	void ShuffleSampleIndices();

	virtual Point2 genSampleOnUnitSquare() = 0;

protected:
	unsigned int numSets;
	unsigned int numSamples;
	Point2* samples;
};
