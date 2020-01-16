#pragma once

#include "Materials/Color.h"
#include "Math/Vector3.h"
#include "SceneData/IntersectionResult.h"

class BRDF {
public:
	virtual Color GetRadiance(const IntersectionResult& IntersectionResult,
		const Vector3 &wo) = 0;
	virtual Color GetRho(const IntersectionResult& IntersectionResult,
		const Vector3 &wo) = 0;
private:
};
