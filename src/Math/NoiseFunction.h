#pragma once

#include "Math/Point3.h"
#include "Math/Vector3.h"

class NoiseFunction {
public:
	virtual ~NoiseFunction() {
	}
	
	virtual float GetValueFBM(Point3 const & point) const = 0;
	virtual Vector3 GetVectorValueFBM(Point3 const & point) const = 0;
	virtual float GetValueTurbulenceFBM(Point3 const & point) const = 0;
	
	virtual float GetValueFractalSum(Point3 const & point) const = 0;
	virtual Vector3 GetVectorFractalSum(Point3 const & point) const = 0;
	
	virtual float GetValueInterpolated(Point3 const & point) const = 0;
	virtual Vector3 GetVectorValueInterpolated(Point3 const & point) const = 0;
};
