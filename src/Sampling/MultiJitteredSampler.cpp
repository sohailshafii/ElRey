#include "MultiJitteredSampler.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

MultiJitteredSampler::MultiJitteredSampler(unsigned int numSets, unsigned int numSamples)
	: GenericMultiSampler(numSets, numSamples) {
	GenerateSamples();
}

MultiJitteredSampler::~MultiJitteredSampler() {
}

// samples are randomly generated per sqrt(n)xsqrt(n) cell,
// and satisfy n-rooks on nxn "sub" grid
void MultiJitteredSampler::GenerateSamples() {
	unsigned int n = (int)sqrt((float)numSamples);
	if (n * n != numSamples) {
		std::cerr << "Num samples for MultiJittered sampler must be " <<
			"perfect square!\n";
		throw std::runtime_error("Num samples for MultiJittered sampler must be \
			perfect square!");
	}
	// TODO
}
