#include "LinearNoiseFunction.h"
#include "Math/CommonMath.h"
#include "Sampling/MultiJitteredSampler.h"
#include <cstdlib>

// Darwyn Peachey in Ebert et al. (2003), page 70.
const unsigned char LinearNoiseFunction::permutationTable[noiseTableSize] =
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

LinearNoiseFunction::LinearNoiseFunction(unsigned int numOctaves,
										 float lacunarity,
										 float gain)
	: numOctaves(numOctaves), lacunarity(lacunarity), gain(gain) {
	InitValueTable();
	InitVectorTable();
	ComputeFBMBounds();
}

float LinearNoiseFunction::GetValueFBM(Point3 const & point) const {
	float amplitude = 1.0f;
	float frequency = 1.0f;
	float sum = 0.0f;
	
	for (int j = 0; j < numOctaves; j++) {
		sum += amplitude * GetValueInterpolated(point * frequency);
		amplitude *= gain;
		frequency *= lacunarity;
	}
	
	sum = (sum - fbmMin)/(fbmMax - fbmMin);
	
	return sum;
}

Vector3 LinearNoiseFunction::GetVectorValueFBM(Point3 const & point) const {
	float 		amplitude 	= 1.0;
	float		frequency 	= 1.0;
	Vector3		sum(0.0f, 0.0f, 0.0f);
			
	for (int j = 0; j < numOctaves; j++) {
		sum += GetVectorValueInterpolated(point * frequency) * amplitude;
		amplitude 	*= gain;
		frequency 	*= lacunarity;
	}
		
	return sum;
}

float LinearNoiseFunction::GetValueTurbulenceFBM(Point3 const & point) const {
	float amplitude = 1.0f;
	float frequency = 1.0f;
	float turbulence = 0.0f;
	
	for (int j = 0; j < numOctaves; j++) {
		turbulence += amplitude * fabs(GetValueInterpolated(point * frequency));
		amplitude *= 0.5f;
		frequency *= 2.0f;
	}
	
	turbulence /= fbmMax;
	
	return turbulence;
}

float LinearNoiseFunction::GetValueFractalSum(Point3 const & point) const {
	float amplitude = 1.0f;
	float frequency = 1.0f;
	float sum = 0.0f;
	
	for (int j = 0; j < numOctaves; j++) {
		sum += amplitude * GetValueInterpolated(point * frequency);
		amplitude *= 0.5f;
		frequency *= 2.0f;
	}
	
	sum = (sum - fbmMin)/(fbmMax - fbmMin);
	
	return sum;
}

Vector3 LinearNoiseFunction::GetVectorFractalSum(Point3 const & point) const {
	float 		amplitude 	= 1.0;
	float		frequency 	= 1.0;
	Vector3		sum(0.0f, 0.0f, 0.0f);
			
	for (int j = 0; j < numOctaves; j++) {
		sum += GetVectorValueInterpolated(point * frequency) * amplitude;
		amplitude 	*= 0.5f;
		frequency 	*= 2.0f;
	}
		
	return sum;
}

void LinearNoiseFunction::InitValueTable() {
	srand(seedValue);
	// values are in the range [-1, +1]
	for (unsigned int i = 0; i < noiseTableSize; i++) {
		noiseValues[i] = 1.0 - 2.0 * CommonMath::RandomUnitValue();
	}
}

// borrowed technique from Ray Tracing from the Ground Up
// The formulae for mapping points from a unit square a unit sphere are in Pharr and Humphreys pp 651-652.
void LinearNoiseFunction::InitVectorTable() {
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
}

void LinearNoiseFunction::ComputeFBMBounds() {
	if (gain == 1.0) {
		fbmMax = (float)numOctaves;
	}
	else {
		fbmMax = (1.0f - pow(gain, numOctaves))/
			(1.0f - gain);
	}
	
	fbmMin = -fbmMax;
}

float LinearNoiseFunction::GetValueInterpolated(Point3 const & point) const {
	unsigned int 	ix, iy, iz;
	float 	fx, fy, fz;
	float 	d[2][2][2];
	float 	x0, x1, x2, x3, y0, y1, z0;
	
	ix = floor(point[0]);
	fx = point[0] - ix;
	
	iy = floor(point[1]);
	fy = point[1] - iy;
	
	iz = floor(point[2]);
	fz = point[2] - iz;
	
	for (int k = 0; k <= 1; k++) {
		for (int j = 0; j <= 1; j++) {
			for (int i = 0; i <= 1; i++) {
				d[k][j][i] =
				noiseValues[INDEX(ix + i, iy + j, iz + k)];
			}
		}
	}
		
	x0 = Lerp(fx, d[0][0][0], d[0][0][1]);
	x1 = Lerp(fx, d[0][1][0], d[0][1][1]);
	x2 = Lerp(fx, d[1][0][0], d[1][0][1]);
	x3 = Lerp(fx, d[1][1][0], d[1][1][1]);
	y0 = Lerp(fy, x0, x1);
	y1 = Lerp(fy, x2, x3);
	z0 = Lerp(fz, y0, y1);
	
	return z0;
}

Vector3 LinearNoiseFunction::GetVectorValueInterpolated(Point3 const & point) const {
	unsigned int 	ix, iy, iz;
	float 	fx, fy, fz;
	Vector3 	d[2][2][2];
	Vector3 	x0, x1, x2, x3, y0, y1, z0;
	
	ix = floor(point[0]);
	fx = point[0] - ix;
	
	iy = floor(point[1]);
	fy = point[1] - iy;
	
	iz = floor(point[2]);
	fz = point[2] - iz;
	
	for (int k = 0; k <= 1; k++) {
		for (int j = 0; j <= 1; j++) {
			for (int i = 0; i <= 1; i++) {
				d[k][j][i] =
				noiseVectorValues[INDEX(ix + i, iy + j, iz + k)];
			}
		}
	}
		
	x0 = LerpVector(fx, d[0][0][0], d[0][0][1]);
	x1 = LerpVector(fx, d[0][1][0], d[0][1][1]);
	x2 = LerpVector(fx, d[1][0][0], d[1][0][1]);
	x3 = LerpVector(fx, d[1][1][0], d[1][1][1]);
	y0 = LerpVector(fy, x0, x1);
	y1 = LerpVector(fy, x2, x3);
	z0 = LerpVector(fz, y0, y1);
	
	return z0;
}



