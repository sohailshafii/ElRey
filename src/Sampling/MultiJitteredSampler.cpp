#include "MultiJitteredSampler.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

MultiJitteredSampler::MultiJitteredSampler(unsigned int numSets,
	unsigned int numSamples)
	: GenericMultiSampler(numSets, numSamples) {
	GenerateSamples();
}

MultiJitteredSampler::~MultiJitteredSampler() {
}

// samples are randomly generated per sqrt(n)xsqrt(n) cell,
// and satisfy n-rooks on nxn "sub" grid
void MultiJitteredSampler::GenerateSamples() {
	unsigned int gridDim = (int)sqrt((float)numSamples);
	if (gridDim * gridDim != numSamples) {
		std::cerr << "Num samples for MultiJittered sampler must be " <<
			"perfect square!\n";
		throw std::runtime_error("Num samples for MultiJittered sampler must be \
			perfect square!");
	}
	float subCellWidth = 1.0f /((float)numSamples);
	
	// set up n-rooks in subgrid, and jitter them (which will be jittered w.r.t.
	// grid size)
	for (unsigned int setIndex = 0; setIndex < numSets; setIndex++) {
		unsigned int setOffset = setIndex * numSamples;
		for (unsigned int gridI = 0, oneDimIndex = 0; gridI < gridDim;
			gridI++) {
			for (unsigned int gridJ = 0; gridJ < gridDim; gridJ++,
				oneDimIndex++) {
				samples[oneDimIndex + setOffset][0] =
					(float)(gridI * gridDim + gridJ) * subCellWidth +
					CommonMath::RandomUnitValue() * subCellWidth;
				samples[oneDimIndex + setOffset][1] =
					(float)(gridJ * gridDim + gridI) * subCellWidth +
					CommonMath::RandomUnitValue() * subCellWidth;
			}
		}
	}

	// shuffle x
	for (unsigned int setIndex = 0; setIndex < numSets; setIndex++) {
		unsigned int setOffset = setIndex * numSamples;
		for (unsigned int gridRow = 0; gridRow < gridDim;
			gridRow++) {
			for (unsigned int gridCol = 0; gridCol < gridDim; gridCol++) {
				unsigned int randomX = (unsigned int)CommonMath::RandInt((int)gridCol, (int)gridDim - 1);
				unsigned int tempIndex = gridRow * gridDim + gridCol + setOffset;
				unsigned int otherIndex = gridRow * gridDim + randomX + setOffset;
				float t = samples[tempIndex][0];
				samples[tempIndex][0] = samples[otherIndex][0];
				samples[otherIndex][0] = t;
			}
		}
	}

	// shuffle y
	for (unsigned int setIndex = 0; setIndex < numSets; setIndex++) {
		unsigned int setOffset = setIndex * numSamples;
		for (unsigned int gridCol = 0; gridCol < gridDim;
			gridCol++) {
			for (unsigned int gridRow = 0; gridRow < gridDim; gridRow++) {
				unsigned int randomY = (unsigned int)CommonMath::RandInt((int)gridRow, (int)gridDim - 1);
				unsigned int tempIndex = gridRow * gridDim + gridCol + setOffset;
				unsigned int otherIndex = randomY * gridDim + gridCol + setOffset;
				float t = samples[tempIndex][1];
				samples[tempIndex][1] = samples[otherIndex][1];
				samples[otherIndex][1] = t;
			}
		}
	}
}
