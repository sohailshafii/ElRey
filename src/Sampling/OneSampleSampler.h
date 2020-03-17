#pragma once

#include "GenericSampler.h"

class OneSampleSampler : public GenericSampler {
public:
	OneSampleSampler();
	~OneSampleSampler();

	virtual void MapSamplesToUnitDisk() override;
	virtual void MapSamplesToHemisphere(float exponent) override;
	
	virtual Point2 GetSampleOnUnitSquare() override;
	virtual Point2 GetSampleOnUnitDisk() override;
	virtual Point3 GetSampleOnHemisphere() override;
};

