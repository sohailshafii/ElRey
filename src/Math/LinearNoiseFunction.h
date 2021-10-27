#pragma once

#include "Math/NoiseFunction.h"
#include "Math/Vector3.h"

const unsigned int noiseTableSize = 256;
const unsigned int noiseTableMask = noiseTableSize - 1;
const unsigned int seedValue = 253;

#define PERM(x)          permutationTable[(x)&noiseTableMask]
#define INDEX(ix,iy,iz)  PERM((ix)+PERM((iy)+PERM(iz)))
#define FLOOR(x) 		 ((int)(x) - ((x) < 0 && (x) != (int) (x)))

class LinearNoiseFunction : public NoiseFunction {
public:
	// For fractal sum and turbulence, gain must be 0.5 and lacunarity 2.0
	LinearNoiseFunction(unsigned int numOctaves,
						float lacunarity,
						float gain);
	virtual float GetValueFBM(Point3 const & point) const override;
	virtual Vector3 GetVectorValueFBM(Point3 const & point) const override;
	virtual float GetValueTurbulenceFBM(Point3 const & point) const override;
	
	virtual float GetValueFractalSum(Point3 const & p) const override;
	virtual Vector3 GetVectorFractalSum(Point3 const & p) const override;
	
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
	
	virtual float GetValueInterpolated(Point3 const & point) const override;
	virtual Vector3 GetVectorValueInterpolated(Point3 const & point) const override;
	
protected:
	unsigned int numOctaves;
	float lacunarity;
	float gain;
	
	float noiseValues[noiseTableSize];
	Vector3 noiseVectorValues[noiseTableSize];
	static const unsigned char permutationTable[noiseTableSize];
	
private:
	float fbmMin, fbmMax;
	
	void InitValueTable();
	void InitVectorTable();
	void ComputeFBMBounds();
	
	float Lerp(float t, float a, float b) const {
		return (a + t * (b - a));
	}
	
	Vector3 LerpVector(float t, Vector3 const &a, Vector3 const &b) const {
		return Vector3(a[0] + t * (b[0] - a[0]),
					   a[1] + t * (b[1] - a[1]),
					   a[2] + t * (b[2] - a[2]));
	}
};
