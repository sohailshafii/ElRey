#include "GenericSampler.h"
#include "CommonMath.h"
#include <algorithm>

GenericSampler::GenericSampler() {
	this->numSets = 1;
	this->numSamples = 83;
	this->count = 0;
	this->jump = 0;
	this->samples = nullptr;
	CreateShuffledIndices();
}

GenericSampler::GenericSampler(unsigned int numSets, unsigned int numSamples) {
	this->numSets = numSets;
	this->numSamples = numSamples;
	this->count = 0;
	this->jump = 0;
	this->samples = new Point2[numSets * numSamples];
	CreateShuffledIndices();
}

GenericSampler::~GenericSampler() {
	if (samples != nullptr) {
		delete[] samples;
	}
}

void GenericSampler::CreateShuffledIndices() {
	shuffledIndices = new unsigned int[numSamples * numSets];
	unsigned int* indices = new unsigned int[numSamples];

	for (unsigned int j = 0; j < numSamples; j++) {
		indices[j] = j;
	}

	for (unsigned int setIndex = 0, oneDimIndex = 0; setIndex < numSets; setIndex++) {
		std::random_shuffle(&indices[0], &indices[numSamples - 1]);

		for (unsigned int j = 0; j < numSamples; j++, oneDimIndex++) {
			shuffledIndices[oneDimIndex] = indices[j];
		}
	}

	delete [] indices;
}

Point2 GenericSampler::SampleUnitSphere() {
	if ((count % numSamples) == 0) {
		jump = (CommonMath::RandInt() % numSamples) * numSamples;
	}
	return (samples[jump + shuffledIndices[jump +
		count++ % numSamples]]);
}
