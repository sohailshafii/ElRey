#pragma once

#include "Materials/Color3.h"
#include "SceneData/ShadingInfo.h"

class AbstractTexture {
public:
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const = 0;
};
