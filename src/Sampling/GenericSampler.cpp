#include "GenericSampler.h"
#include <cstring>
#include <iostream>

GenericSampler::GenericSampler() : numSets(83), numSamples(1), samples(nullptr) {
	AllocateGenericSamples();
}

GenericSampler::GenericSampler(unsigned int numSamples) : numSets(83), numSamples(numSamples),
  samples(nullptr) {
	AllocateGenericSamples();
}

GenericSampler::GenericSampler(unsigned int numSets, unsigned int numSamples) :
	numSets(numSets), numSamples(numSamples), samples(nullptr) {
	AllocateGenericSamples();
}

GenericSampler::GenericSampler(const GenericSampler &other) {
	numSets = other.numSets;
	numSamples = other.numSamples;
	AllocateGenericSamples();
	for (unsigned int i = 0; i < numSets*numSamples; i++) {
		samples[i] = other.samples[i];
	}
}

void GenericSampler::AllocateGenericSamples() {
	this->samples = new Point2[numSets * numSamples];
}

GenericSampler::~GenericSampler() {
	if (this->samples != nullptr) {
		delete[] this->samples;
	}
}
