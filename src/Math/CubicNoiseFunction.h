#pragma once

#include "Math/LinearNoiseFunction.h"

class CubicNoiseFunction : public LinearNoiseFunction {
public:
	// For fractal sum and turbulence, gain must be 0.5 and lacunarity 2.0
	CubicNoiseFunction(unsigned int numOctaves,
					   float lacunarity,
					   float gain);
	
	virtual float GetValueInterpolated(Point3 const & point) const override;
	virtual Vector3 GetVectorValueInterpolated(Point3 const & point) const override;
	
private:
	
	float FourKnotSpline(float x, float knots[]) const {
		float c3 = -0.5 * knots[0] + 1.5 * knots[1] - 1.5 * knots[2] + 0.5 * knots[3];
		float c2 = knots[0] - 2.5 * knots[1] + 2.0 * knots[2] - 0.5 * knots[3];
		float c1 = 0.5 * (-knots[0] + knots [2]);
		float c0 = knots[1];
		 
		return ((c3*x + c2)*x + c1)*x + c0;
	}
	
	Vector3 FourKnotSplineVec(float x, Vector3 knots[]) const {
		Vector3 c3 = knots[0] * -0.5f + knots[1] * 1.5f - knots[2] * 1.5f + knots[3] * 0.5f;
		Vector3 c2 = knots[0] - knots[1] * 2.5f + knots[2] * 2.0f - knots[3] * 0.5f;
		Vector3 c1 = (-knots[0] + knots [2]) * 0.5f;
		Vector3 c0 = knots[1];
		 
		return ((c3*x + c2)*x + c1)*x + c0;
	}
};
