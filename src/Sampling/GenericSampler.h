#pragma once

#include "Math/Point2.h"

class GenericSampler {
public:
	virtual Point2 genSampleOnUnitSquare() = 0;
};
