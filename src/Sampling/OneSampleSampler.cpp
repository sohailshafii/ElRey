#include "OneSampleSampler.h"

// just one point in the center of normalized space
OneSampleSampler::OneSampleSampler() : GenericSampler(1, 1) {
	samples[0] = Point2(0.0f, 0.0f);
}

OneSampleSampler::~OneSampleSampler() {
}

Point2 OneSampleSampler::GetSampleOnUnitSquare() {
	return samples[0];
}

Point2 OneSampleSampler::GetSampleOnUnitDisk() {
	return samples[0];
}
