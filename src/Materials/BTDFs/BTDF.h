#pragma once

#include "Materials/Color3.h"
#include "Math/Vector3.h"
#include "SceneData/ShadingInfo.h"

class BTDF {
public:
	virtual ~BTDF() {
	}
	
	virtual Color3 SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &transmittedVec,
						   float &transmission, float etaOut = 1.0f) const = 0;
	
	virtual bool AllowsTransmission(ShadingInfo const & shadingInfo,
									float etaOut = 1.0f) const = 0;
private:
};
