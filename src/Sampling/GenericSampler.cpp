#include "GenericSampler.h"
#include <algorithm>

GenericSampler::GenericSampler() {
	this->numSets = 1;
	this->numSamples = 83;
	this->count = 0;
	this->jump = 0;
	this->samples = nullptr;
	this->diskSamples = nullptr;
	this->shuffledIndices = nullptr;
	AllocateSamples();
	CreateShuffledIndices();
}

GenericSampler::GenericSampler(unsigned int numSets, unsigned int numSamples) {
	this->numSets = numSets;
	this->numSamples = numSamples;
	this->count = 0;
	this->jump = 0;
	this->samples = nullptr;
	this->diskSamples = nullptr;
	this->shuffledIndices = nullptr;
	AllocateSamples();
	CreateShuffledIndices();
}

void GenericSampler::AllocateSamples() {
	this->samples = new Point2[numSets * numSamples];
	this->diskSamples = new Point2[numSets * numSamples];
}

GenericSampler::~GenericSampler() {
	if (samples != nullptr) {
		delete[] samples;
	}
	if (diskSamples != nullptr) {
		delete[] diskSamples;
	}
	if (shuffledIndices != nullptr) {
		delete[] shuffledIndices;
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

Point2 GenericSampler::GetSampleOnUnitSquare() {
	CheckForNewJumpValue();
	return samples[GetNewSampleIndex()];
}

Point2 GenericSampler::GetSampleOnUnitDisk() {
	CheckForNewJumpValue();
	return diskSamples[GetNewSampleIndex()];
}
