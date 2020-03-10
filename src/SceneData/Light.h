#pragma once

#include "Math/Vector3.h"
#include "Math/Point3.h"
#include "Materials/Color3.h"

class Light {
public:
	Light() {
		
	}
	virtual ~Light() {
		
	}
	
	virtual Vector3 GetDirectionFromPosition(
		const Point3& worldPosition) = 0;
	virtual Color3 GetRadiance() = 0;
	virtual bool CastsShadows() = 0;
	
	virtual bool IsAmbient() const {
		return false;
	}
};
