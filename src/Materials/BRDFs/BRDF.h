#pragma once

#include "Materials/Color.h"
#include "Math/Vector3.h"
#include "SceneData/IntersectionResult.h"

class BRDF {
public:
	virtual Color GetRadiance(const IntersectionResult& IntersectionResult) = 0;
	virtual Color GetRho(const IntersectionResult& IntersectionResult) = 0;
private:
};
