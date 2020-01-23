#pragma once

#include "Materials/Color.h"
#include "SceneData/IntersectionResult.h"

class Material {
public:
	virtual Color GetAmbientColor(const IntersectionResult &intersectionResult) = 0;
	virtual Color GetDirectColor(const IntersectionResult &intersectionResult) = 0;
};
