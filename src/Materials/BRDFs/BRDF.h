#pragma once

#include "Materials/Color3.h"
#include "Math/Vector3.h"
#include "SceneData/IntersectionResult.h"

class BRDF {
public:
	virtual Color3 GetRadiance(const IntersectionResult& IntersectionResult) const  = 0;
	virtual Color3 GetRho(const IntersectionResult& IntersectionResult) const  = 0;
private:
};
