#pragma once

#include "Math/Point2.h"
#include "Math/Point3.h"

class GenericSampler {
public:
	GenericSampler();
	GenericSampler(unsigned int numSamples);
	GenericSampler(unsigned int numSets, unsigned int numSamples);

	virtual ~GenericSampler();
	
	virtual void MapSamplesToUnitDisk() = 0;
	virtual void MapSamplesToHemisphere(float exponent) = 0;
	
	virtual Point2 GetSampleOnUnitSquare() = 0;
	virtual Point2 GetSampleOnUnitDisk() = 0;
	virtual Point3 GetSampleOnHemisphere() = 0;
	
	unsigned int GetNumSets() const {
		return numSets;
	}
	
	unsigned int GetNumSamples() const {
		return numSamples;
	}

protected:
	unsigned int numSets;
	unsigned int numSamples;
	Point2* samples;

private:
	void AllocateGenericSamples();
};
