#pragma once

#include "GenericSampler.h"
#include "Math/Point2.h"
#include "CommonMath.h"

class GenericMultiSampler : public GenericSampler {
public:
	GenericMultiSampler();
	GenericMultiSampler(unsigned int numSamples);
	GenericMultiSampler(unsigned int numSets, unsigned int numSamples);

	virtual ~GenericMultiSampler();

	Point2 GetSampleOnUnitSquare() override;
	Point2 GetSampleOnUnitDisk() override;

protected:
	// count is the number of sample points used
	unsigned long count;
	unsigned int jump;

	Point2* diskSamples;
	unsigned int* shuffledIndices;

	void InitializeMultiSampler();
	void AllocateSamples();
	void CreateShuffledIndices();
	void MapSamplesToUnitDisk();

	void CheckForNewJumpValue() {
		if ((count % numSamples) == 0) {
			jump = (CommonMath::RandInt() % numSets) * numSamples;
		}
	}

	unsigned int GetNewSampleIndex() {
		return jump + shuffledIndices[jump + count++ % numSamples];
	}
};
