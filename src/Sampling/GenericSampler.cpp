#include "GenericSampler.h"
#include <algorithm>
#include <cmath>

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

void GenericSampler::MapSamplesToUnitDisk() {
	float r, phi;
	Point2 samplePnt;

	for (int sampleIndex = 0; sampleIndex < numSamples; sampleIndex++) {
		// go from quadrant to quarter
		auto& sample = samples[sampleIndex];
		samplePnt[0] = 2.0f * sample[0] - 1.0f;
		samplePnt[1] = 2.0f * sample[1] - 1.0f;

		float x = samplePnt[0];
		float y = samplePnt[1];
		if (x > -y) { // sectors 1 and 2
			if (x > y) { // sector 1
				r = x;
				phi = y / x;
			}
			else {
				r = x;
				phi = 2.0f - x / y;
			}
		}
		else {
			if (x < y) {
				r = -x;
				phi = 4.0f + y / x;
			}
			else {
				r = -y;
				if (fabs(y) > EPSILON) {
					phi = 6.0f - x / y;
				}
				else {
					phi = 0.0f;
				}
			}
		}

		phi *= M_PI / 4.0f;
		diskSamples[sampleIndex][0] = r * cos(phi);
		diskSamples[sampleIndex][1] = r * sin(phi);
	}
}

Point2 GenericSampler::GetSampleOnUnitSquare() {
	CheckForNewJumpValue();
	return samples[GetNewSampleIndex()];
}

Point2 GenericSampler::GetSampleOnUnitDisk() {
	CheckForNewJumpValue();
	return diskSamples[GetNewSampleIndex()];
}
