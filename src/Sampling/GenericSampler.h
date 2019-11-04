#pragma once

#include "Math/Point2.h"
#include "CommonMath.h"

class GenericSampler {
public:
	GenericSampler();
	GenericSampler(unsigned int numSets, unsigned int numSamples);

	virtual ~GenericSampler();

	Point2 GetSampleOnUnitSquare();
	Point2 GetSampleOnUnitDisk();

protected:
	unsigned int numSets;
	unsigned int numSamples;
	// count is the number of sample points used
	unsigned long count;
	unsigned int jump;

	Point2* samples;
	Point2* diskSamples;
	unsigned int* shuffledIndices;

	void AllocateSamples();
	void CreateShuffledIndices();
	void MapSamplesToUnitDisk();

	void CheckForNewJumpValue() {
		if ((count % numSamples) == 0) {
			jump = (CommonMath::RandInt() % numSamples) * numSamples;
		}
	}

	unsigned int GetNewSampleIndex() {
		return jump + shuffledIndices[jump + count++ % numSamples];
	}
};
