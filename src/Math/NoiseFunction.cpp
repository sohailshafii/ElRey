
#include "NoiseFunction.h"
#include "Sampling/MultiJitteredSampler.h"

// Darwyn Peachey in Ebert et al. (2003), page 70.
// A lot of this code was borrowed from Raytracing from the Ground Up, and that code
// was in turn borrowed from other places.
const unsigned char NoiseFunction::permutationTable[noiseTableSize] =
{
	225,155,210,108,175,199,221,144,203,116, 70,213, 69,158, 33,252,
	5, 82,173,133,222,139,174, 27,  9, 71, 90,246, 75,130, 91,191,
	169,138,  2,151,194,235, 81,  7, 25,113,228,159,205,253,134,142,
	248, 65,224,217, 22,121,229, 63, 89,103, 96,104,156, 17,201,129,
	36,  8,165,110,237,117,231, 56,132,211,152, 20,181,111,239,218,
	170,163, 51,172,157, 47, 80,212,176,250, 87, 49, 99,242,136,189,
	162,115, 44, 43,124, 94,150, 16,141,247, 32, 10,198,223,255, 72,
	53,131, 84, 57,220,197, 58, 50,208, 11,241, 28,  3,192, 62,202,
	18,215,153, 24, 76, 41, 15,179, 39, 46, 55,  6,128,167, 23,188,
	106, 34,187,140,164, 73,112,182,244,195,227, 13, 35, 77,196,185,
	26,200,226,119, 31,123,168,125,249, 68,183,230,177,135,160,180,
	12,  1,243,148,102,166, 38,238,251, 37,240,126, 64, 74,161, 40,
	184,149,171,178,101, 66, 29, 59,146, 61,254,107, 42, 86,154,  4,
	236,232,120, 21,233,209, 45, 98,193,114, 78, 19,206, 14,118,127,
	48, 79,147, 85, 30,207,219, 54, 88,234,190,122, 95, 67,143,109,
	137,214,145, 93, 92,100,245,  0,216,186, 60, 83,105, 97,204, 52
};

NoiseFunction::NoiseFunction(unsigned int numOctaves, float lacunarity,
			  float gain, float startingFrequency) : numOctaves(numOctaves),
				lacunarity(lacunarity), gain(gain),
				startingFrequency(startingFrequency) {
  InitValueTable();
  InitVectorTable();
  ComputeFBMBounds();
}

void NoiseFunction::InitValueTable() {
	srand(seedValue);
	// values are in the range [-1, +1]
	for (unsigned int i = 0; i < noiseTableSize; i++) {
		noiseValues[i] = 1.0f - 2.0f * CommonMath::RandomUnitValue();
	}
}

// borrowed technique from Ray Tracing from the Ground Up
// The formulae for mapping points from a unit square a unit sphere are in Pharr and Humphreys pp 651-652.
void NoiseFunction::InitVectorTable() {
	srand(seedValue);
	
	auto *unitSquareSampler = new MultiJitteredSampler(256, 1);
	
	for (unsigned int j = 0; j < noiseTableSize; j++) {
		Point2 samplePoint = unitSquareSampler->Sample2DNoShuffle();
		float r1 = samplePoint[0];
		float r2 = samplePoint[1];
		float z = 1.0f - 2.0f * r1;
		float r = sqrt(1.0f - z * z);
		float phi = TWO_PI * r2;
		float x = r * cos(phi);
		float y = r * sin(phi);
		noiseVectorValues[j] = Vector3(x, y, z).Normalized();
	}
	
	delete unitSquareSampler;
}

void NoiseFunction::ComputeFBMBounds() {
	if (gain == 1.0) {
		fbmMax = (float)numOctaves;
	}
	else {
		fbmMax = (1.0f - pow(gain, numOctaves))/
			(1.0f - gain);
	}
	
	fbmMin = -fbmMax;
}

float NoiseFunction::GetValueFBM(Point3 const & point) const {
	float amplitude = 1.0f;
	float frequency = startingFrequency;
	float sum = 0.0f;
	
	for (int j = 0; j < numOctaves; j++) {
		float interpValue = GetValueInterpolated(point * frequency);
		sum += amplitude * interpValue;
		amplitude *= gain;
		frequency *= lacunarity;
	}
	
	sum = (sum - fbmMin)/(fbmMax - fbmMin);
	
	return sum;
}

Vector3 NoiseFunction::GetVectorValueFBM(Point3 const & point) const {
	float 		amplitude 	= 1.0f;
	float		frequency 	= startingFrequency;
	Vector3		sum(0.0f, 0.0f, 0.0f);
			
	for (int j = 0; j < numOctaves; j++) {
		sum += GetVectorValueInterpolated(point * frequency) * amplitude;
		amplitude 	*= gain;
		frequency 	*= lacunarity;
	}
		
	return sum;
}

float NoiseFunction::GetValueTurbulenceFBM(Point3 const & point) const {
	float amplitude = 1.0f;
	float frequency = startingFrequency;
	float turbulence = 0.0f;
	
	for (int j = 0; j < numOctaves; j++) {
		turbulence += amplitude * fabs(GetValueInterpolated(point * frequency));
		amplitude *= 0.5f;
		frequency *= 2.0f;
	}
	
	turbulence /= fbmMax;
	
	return turbulence;
}

float NoiseFunction::GetValueFractalSum(Point3 const & point) const {
	float amplitude = 1.0f;
	float frequency = startingFrequency;
	float sum = 0.0f;
	
	for (int j = 0; j < numOctaves; j++) {
		sum += amplitude * GetValueInterpolated(point * frequency);
		amplitude *= 0.5f;
		frequency *= 2.0f;
	}
	
	sum = (sum - fbmMin)/(fbmMax - fbmMin);
	
	return sum;
}

Vector3 NoiseFunction::GetVectorFractalSum(Point3 const & point) const {
	float 		amplitude 	= 1.0f;
	float		frequency 	= startingFrequency;
	Vector3		sum(0.0f, 0.0f, 0.0f);
			
	for (int j = 0; j < numOctaves; j++) {
		sum += GetVectorValueInterpolated(point * frequency) * amplitude;
		amplitude 	*= 0.5f;
		frequency 	*= 2.0f;
	}
		
	return sum;
}
