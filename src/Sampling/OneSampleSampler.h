#pragma once

#include "GenericSampler.h"

class OneSampleSampler : public GenericSampler {
public:
	OneSampleSampler();
	~OneSampleSampler();
	
	virtual OneSampleSampler* clone() const override {
		return new OneSampleSampler(*this);
	}

	virtual void MapSamplesToUnitDisk() override;
	virtual void MapSamplesToHemisphere(float exponent) override;
	
	virtual const Point2& GetSampleOnUnitSquare() override;
	virtual const Point2& GetSampleOnUnitDisk() override;
	virtual const Point3& GetSampleOnHemisphere() override;
	
	virtual const Point2& Sample2DNoShuffle() override;

private:
	Point3 dummyPoint;
};

