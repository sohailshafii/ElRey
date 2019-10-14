#pragma once

#include "Math/Point2.h"

class GenericSampler {
public:
	GenericSampler() {
		this->numSets = 1;
		this->numSamples = 83;
		this->count = 0;
		this->jump = 0;
		this->samples = nullptr;
	}

	GenericSampler(unsigned int numSets, unsigned int numSamples) {
		this->numSets = numSets;
		this->numSamples = numSamples;
		this->count = 0;
		this->jump = 0;
		this->samples = new Point2[numSets * numSamples];
	}

	virtual ~GenericSampler() {
		if (samples != nullptr) {
			delete[] samples;
		}
	}

	virtual Point2 GenSampleOnUnitSquare() = 0;
	Point2 SampleUnitSphere();

protected:
	unsigned int numSets;
	unsigned int numSamples;
	// count is the number of sample points used
	unsigned long count;
	unsigned int jump;

	Point2* samples;
	int* shuffledIndices;

	void ShuffleSampleIndices();
	void ShuffleSamples();
	void SetupShuffledIndices();
};
