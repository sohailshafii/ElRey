#pragma once

#include "Math/Point3.h"
#include "Math/Vector3.h"

const unsigned int noiseTableSize = 256;
const unsigned int noiseTableMask = noiseTableSize - 1;
const unsigned int seedValue = 253;

class NoiseFunction {
public:
	NoiseFunction(unsigned int numOctaves, float lacunarity,
				  float gain, float startingFrequency);
	
	virtual ~NoiseFunction() {
	}
	
	float GetValueFBM(Point3 const & point) const;
	Vector3 GetVectorValueFBM(Point3 const & point) const;
	float GetValueTurbulenceFBM(Point3 const & point) const;
	
	float GetValueFractalSum(Point3 const & point) const;
	Vector3 GetVectorFractalSum(Point3 const & point) const;
	
	virtual float GetValueInterpolated(Point3 const & point) const = 0;
	virtual Vector3 GetVectorValueInterpolated(Point3 const & point) const = 0;
	
	void SetNumOctaves(unsigned int numOctaves) {
		this->numOctaves = numOctaves;
		ComputeFBMBounds();
	}
	
	void SetLacunarity(float lacunarity) {
		this->lacunarity = lacunarity;
	}
	
	void SetGain(float gain) {
		this->gain = gain;
		ComputeFBMBounds();
	}
	
protected:
	unsigned int numOctaves;
	float lacunarity;
	float gain;
	float startingFrequency;
	
	float noiseValues[noiseTableSize];
	Vector3 noiseVectorValues[noiseTableSize];
	static const unsigned char permutationTable[noiseTableSize];
	
private:
	float fbmMin, fbmMax;
	
	void InitValueTable();
	void InitVectorTable();
	void ComputeFBMBounds();
};
