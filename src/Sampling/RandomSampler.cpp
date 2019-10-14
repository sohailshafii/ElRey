#include "RandomSampler.h"
#include "Math/CommonMath.h"

RandomSampler::RandomSampler(unsigned int numSets,
	unsigned int numSamples) : GenericSampler(numSets, numSamples) {
	this->numSets = numSets;
	this->numSamples = numSamples;
	samples = new Point2[numSets*numSamples];

	for (unsigned int setIndex = 0, oneDimIndex = 0; setIndex < numSets;
		setIndex++) {
		for (unsigned int sampleIndex = 0; sampleIndex < numSamples;
			sampleIndex++, oneDimIndex++) {
			samples[oneDimIndex] = Point2(CommonMath::randomUnitValue(),
				CommonMath::randomUnitValue());
		}
	}
}

RandomSampler::~RandomSampler() {
	delete [] samples;	
}

Point2 RandomSampler::GenSampleOnUnitSquare() {
	return Point2(0.0f, 0.0f);
}

