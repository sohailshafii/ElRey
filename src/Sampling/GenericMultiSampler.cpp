#include "GenericMultiSampler.h"
#include <algorithm>
#include <cmath>

GenericMultiSampler::GenericMultiSampler() : GenericSampler(),
  count(0), jump(0), diskSamples(nullptr), shuffledIndices(nullptr) {
	InitializeMultiSampler();
}

// I initialize some variables in the constructor because Visual Studio whines that my constructors
// DON'T initialize the variables even though a function that it calls DOES
// initialize them!
GenericMultiSampler::GenericMultiSampler(unsigned int numSamples) : GenericSampler(numSamples),
  count(0), jump(0), diskSamples(nullptr), shuffledIndices(nullptr) {
	InitializeMultiSampler();
}

GenericMultiSampler::GenericMultiSampler(unsigned int numSets, unsigned int numSamples) :
	GenericSampler(numSets, numSamples),count(0), jump(0),
	diskSamples(nullptr), shuffledIndices(nullptr) {
	InitializeMultiSampler();
}

void GenericMultiSampler::InitializeMultiSampler() {
	this->count = 0;
	this->jump = 0;
	this->diskSamples = nullptr;
	this->shuffledIndices = nullptr;
	AllocateSamples();
	CreateShuffledIndices();
}

void GenericMultiSampler::AllocateSamples() {
	this->diskSamples = new Point2[numSets * numSamples];
}

GenericMultiSampler::~GenericMultiSampler() {
	if (diskSamples != nullptr) {
		delete[] diskSamples;
	}
	if (shuffledIndices != nullptr) {
		delete[] shuffledIndices;
	}
}

void GenericMultiSampler::CreateShuffledIndices() {
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

void GenericMultiSampler::MapSamplesToUnitDisk() {
	float r, phi;
	Point2 samplePnt;

	for (unsigned int sampleIndex = 0; sampleIndex < numSamples; sampleIndex++) {
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

		phi *= (float)M_PI / 4.0f;
		diskSamples[sampleIndex][0] = r * cos(phi);
		diskSamples[sampleIndex][1] = r * sin(phi);
	}
}

Point2 GenericMultiSampler::GetSampleOnUnitSquare() {
	CheckForNewJumpValue();
	return samples[GetNewSampleIndex()];
}

Point2 GenericMultiSampler::GetSampleOnUnitDisk() {
	CheckForNewJumpValue();
	return diskSamples[GetNewSampleIndex()];
}