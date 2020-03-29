#include "RegularSampler.h"
#include "Math/CommonMath.h"

RegularSampler::RegularSampler(unsigned int numSets,
	unsigned int numSamples) : GenericMultiSampler(numSets, numSamples) {
	GenerateSamples();
}

RegularSampler::~RegularSampler() {
}

void RegularSampler::GenerateSamples() {
	unsigned int sqrtRtNumSamples =
	(unsigned int)sqrt((float)numSamples);
	for (unsigned int setIndex = 0, oneDimIndex = 0; setIndex < numSets;
		setIndex++) {
		for (unsigned int sqrtIndex2 = 0; sqrtIndex2 < sqrtRtNumSamples;
			sqrtIndex2++) {
			for (unsigned int sqrtIndex1 = 0; sqrtIndex1 < sqrtRtNumSamples; sqrtIndex1++, oneDimIndex++)
			{
				// start half-way in between two samples
				// so if we have 1/sqrtNumSamples across one dimension,
				// add 1/(2*sqrtNumSamples) to first sample, which is also equal
				// to 0.5/(sqrtNumSamples)
				samples[oneDimIndex] = Point2((sqrtIndex1 + 0.5f)/(float)sqrtRtNumSamples,
											  (sqrtIndex2 + 0.5f)/(float)sqrtRtNumSamples);
			}
		}
	}
}

