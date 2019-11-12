#include "RandomSampler.h"
#include "Math/CommonMath.h"

RandomSampler::RandomSampler(unsigned int numSets,
	unsigned int numSamples) : GenericMultiSampler(numSets, numSamples) {
	GenerateSamples();
}

RandomSampler::~RandomSampler() {
}

void RandomSampler::GenerateSamples() {
	for (unsigned int setIndex = 0, oneDimIndex = 0; setIndex < numSets;
		setIndex++) {
		for (unsigned int sampleIndex = 0; sampleIndex < numSamples;
			sampleIndex++, oneDimIndex++) {
			samples[oneDimIndex] = Point2(CommonMath::RandomUnitValue(),
				CommonMath::RandomUnitValue());
		}
	}
}


