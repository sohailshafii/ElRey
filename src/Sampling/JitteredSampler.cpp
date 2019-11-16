#include "JitteredSampler.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

JitteredSampler::JitteredSampler(unsigned int numSets, unsigned int numSamples)
	: GenericMultiSampler(numSets, numSamples) {
	GenerateSamples();
}

JitteredSampler::~JitteredSampler() {
}

// Samples are on a grid, but perturbed randomly per grid cell.
void JitteredSampler::GenerateSamples() {
	unsigned int n = (int)sqrt((float)numSamples);
	if (n * n != numSamples) {
		std::cerr << "Num samples for Jittered sampler must be " <<
			"perfect square!\n";
		throw std::runtime_error("Num samples for Jittered sampler must be \
			perfect square!");
	}

	for (unsigned int setIndex = 0, oneDimIndex = 0; setIndex < numSets;
		setIndex++) {
		for (unsigned int row = 0; row < n; row++) {
			for (unsigned int col = 0; col < n; col++, oneDimIndex++) {
				samples[oneDimIndex] = Point2(
					((float)col + CommonMath::RandomUnitValue())/(float)n,
					((float)row + CommonMath::RandomUnitValue())/(float)n);
			}
		}
	}
}
