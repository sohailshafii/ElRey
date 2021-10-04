#pragma once

#include "Math/Point3.h"
#include "Math/Vector3.h"

class NoiseFunction {
public:
	virtual ~NoiseFunction() {
	}
	
	virtual float GetValue(Point3 const & point) const = 0;
	virtual Vector3 GetVectorValue(Point3 const & point) const = 0;
	virtual float GetValueTurbulence(Point3 const & point) const = 0;
};
