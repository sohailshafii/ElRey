#pragma once

#include "GenericSampler.h"
#include "CommonMath.h"

class GenericMultiSampler : public GenericSampler {
public:
	GenericMultiSampler();
	GenericMultiSampler(unsigned int numSamples);
	GenericMultiSampler(unsigned int numSets, unsigned int numSamples);

	virtual ~GenericMultiSampler();

	virtual void MapSamplesToUnitDisk() override;
	virtual void MapSamplesToHemisphere(float exponent) override;
	
	virtual Point2 GetSampleOnUnitSquare() override;
	virtual Point2 GetSampleOnUnitDisk() override;
	virtual Point3 GetSampleOnHemisphere() override;

protected:
	// count is the number of sample points used
	unsigned long count;
	unsigned int jump;

	Point2* diskSamples;
	Point3* hemisphereSamples;
	unsigned int* shuffledIndices;

	void InitializeMultiSampler();
	void AllocateSamples();
	void CreateShuffledIndices();

	void CheckForNewJumpValue() {
		if ((count % numSamples) == 0) {
			jump = (CommonMath::RandInt() % numSets) * numSamples;
		}
	}

	unsigned int GetNewSampleIndex() {
		return jump + shuffledIndices[jump + count++ % numSamples];
	}
};
