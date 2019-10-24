#include "GenericSampler.h"
#include <algorithm>

void GenericSampler::SetupShuffledIndices() {
	shuffledIndices = new unsigned int[numSamples * numSets];
	unsigned int* indices = new unsigned int[numSamples];

	for (unsigned int j = 0; j < numSets; j++) {
		indices[j] = j;
	}

	for (int setIndex = 0; setIndex < numSets; setIndex++) {

	}

	delete indices;
}

Point2 GenericSampler::SampleUnitSphere() {

}

//
void GenericSampler::SetupShuffledIndices() {
	if (shuffledIndices != nullptr) {
		delete [] shuffledIndices;
	}
	shuffledIndices = new unsigned int[numSamples*numSets];
	unsigned int* indices = new unsigned int[numSamples];

	for (unsigned int index = 0; index < numSamples; index++) {
		indices[index] = index;
	}

	unsigned int shuffledIndex = 0;
	for (unsigned int setIndex = 0; setIndex < numSets; setIndex++) {
		random_shuffle(&indices[0], &indices[numSamples-1]);

		for (unsigned int sampleIndex = 0; sampleIndex < numSamples;
			sampleIndex++, shuffledIndex++) {
			shuffledIndices[shuffledIndex] = indices;
		}
	}
}