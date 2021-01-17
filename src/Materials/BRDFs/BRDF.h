#pragma once

#include "Materials/Color3.h"
#include "Math/Vector3.h"
#include "SceneData/ShadingInfo.h"

class BRDF {
public:
	virtual Color3 GetRadiance(const ShadingInfo& shadingInfo) const  = 0;
	virtual Color3 GetRho(const ShadingInfo& shadingInfo) const  = 0;
private:
};
