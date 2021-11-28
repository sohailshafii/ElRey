#pragma once

#include "Math/NoiseFunction.h"
#include "Math/Vector3.h"


#define PERM(x)          permutationTable[(x)&noiseTableMask]
#define INDEX(ix,iy,iz)  PERM((ix)+PERM((iy)+PERM(iz)))
#define FLOOR(x) 		 ((int)(x) - ((x) < 0 && (x) != (int) (x)))

class LinearNoiseFunction : public NoiseFunction {
public:
	// For fractal sum and turbulence, gain must be 0.5 and lacunarity 2.0
	LinearNoiseFunction(unsigned int numOctaves,
						float lacunarity,
						float gain,
						float startingFrequency);
	
	virtual float GetValueInterpolated(Point3 const & point) const override;
	virtual Vector3 GetVectorValueInterpolated(Point3 const & point) const override;
	
private:
	
	
	float Lerp(float t, float a, float b) const {
		return (a + t * (b - a));
	}
	
	Vector3 LerpVector(float t, Vector3 const &a, Vector3 const &b) const {
		return Vector3(a[0] + t * (b[0] - a[0]),
					   a[1] + t * (b[1] - a[1]),
					   a[2] + t * (b[2] - a[2]));
	}
};
