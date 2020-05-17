#pragma once

#include "Materials/Color.h"
#include "SceneData/IntersectionResult.h"

class Material {
public:
	Material() {
		
	}
	virtual ~Material() {
		
	}
	
	virtual Color GetAmbientColor(const IntersectionResult &intersectionResult) = 0;
	virtual Color GetDirectColor(const IntersectionResult &intersectionResult) = 0;
	virtual Color GetColorForAreaLight(const IntersectionResult& intersectionResult) = 0;
};
