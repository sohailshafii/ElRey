#pragma once

#include "Materials/Color.h"
#include "SceneData/IntersectionResult.h"

class Material {
public:
	virtual Color GetColor(const IntersectionResult &intersectionResult) = 0;
};
