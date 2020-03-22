#pragma once

#include "Math/Vector3.h"
#include "Math/Point3.h"
#include "Materials/Color3.h"
#include "IntersectionResult.h"

class Light {
public:
	Light(bool castsShadows) : castsShadows(castsShadows) {
		
	}
	virtual ~Light() {
		
	}
	
	virtual Vector3 GetDirectionFromPosition(
		const IntersectionResult& intersectionRes) = 0;
	virtual Color3 GetRadiance() = 0;
	bool CastsShadows() {
		return castsShadows;
	}
	
	virtual bool IsAmbient() const {
		return false;
	}
	
	virtual bool IsLightDistanceInfinite() const {
		return false;
	}
	
private:
	bool castsShadows;
};
