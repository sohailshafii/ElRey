#include "GenericSampler.h"


GenericSampler::GenericSampler() : numSets(0), numSamples(0), samples(nullptr) {
	AllocateGenericSamples(1, 83);
}

GenericSampler::GenericSampler(unsigned int numSamples) : numSets(0), numSamples(0),
  samples(nullptr) {
	AllocateGenericSamples(1, numSamples);
}

GenericSampler::GenericSampler(unsigned int numSets, unsigned int numSamples) :
	numSets(0), numSamples(0), samples(nullptr) {
	AllocateGenericSamples(numSets, numSamples);
}

void GenericSampler::AllocateGenericSamples(unsigned int numSets, unsigned int numSamples) {
	this->numSets = numSets;
	this->numSamples = numSamples;
	this->samples = new Point2[numSets * numSamples];
}

GenericSampler::~GenericSampler() {
	if (this->samples != nullptr) {
		delete[] this->samples;
	}
}
