#pragma once

#include "GenericSampler.h"

class OneSampleSampler : public GenericSampler {
public:
	OneSampleSampler();
	~OneSampleSampler();

	void MapSamplesToUnitDisk() override;
	Point2 GetSampleOnUnitSquare() override;
	Point2 GetSampleOnUnitDisk() override;
};

