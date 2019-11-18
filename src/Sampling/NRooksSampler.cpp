#include "NRooksSampler.h"

NRooksSampler::NRooksSampler(unsigned int numSets,
	unsigned int numSamples) : GenericMultiSampler(numSets, numSamples) {
	GenerateSamples();
}

NRooksSampler::~NRooksSampler() {
}

void NRooksSampler::GenerateSamples() {
	for (unsigned int setIndex = 0, oneDimIndex = 0; setIndex < numSets;
		setIndex++) {
		for (unsigned int sampleIndex = 0; sampleIndex < numSamples;
			sampleIndex++, oneDimIndex++) {
			samples[oneDimIndex] = Point2(
				((float)sampleIndex + CommonMath::RandomUnitValue())/(float)numSamples,
				((float)sampleIndex + CommonMath::RandomUnitValue())/(float)numSamples);
		}
	}
}

void NRooksSampler::ShuffleSamplesCoords(bool swapX) {
	unsigned int indexToSwap = swapX ? 0 : 1;
	for (unsigned int setIndex = 0; setIndex < numSets; setIndex++) {
		for (unsigned int sampleIndex = 0; sampleIndex < numSamples - 1;
			sampleIndex++) {
			unsigned int newIndex = CommonMath::RandInt() % numSamples +
				setIndex * numSamples;
			unsigned int followingIndex = sampleIndex + setIndex * numSamples + 1;
			float oldVal = samples[followingIndex][indexToSwap];
			samples[followingIndex][indexToSwap] = samples[newIndex][indexToSwap];
			samples[newIndex][indexToSwap] = oldVal;
		}
	}
}
