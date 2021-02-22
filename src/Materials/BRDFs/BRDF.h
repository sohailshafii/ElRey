#pragma once

#include "Materials/Color3.h"
#include "Math/Vector3.h"
#include "SceneData/ShadingInfo.h"

class BRDF {
public:
	virtual ~BRDF() {
		
	}
	
	// GetRadiance might modify incoming direction, so its argument
	// is not const
	virtual Color3 F(ShadingInfo& shadingInfo) const = 0;
	virtual Color3 SampleF(ShadingInfo& shadingInfo, float& pdf) const = 0;
	virtual Color3 GetRho(const ShadingInfo& shadingInfo) const  = 0;
private:
};
