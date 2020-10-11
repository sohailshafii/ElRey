#pragma once

#include "Materials/Color.h"
#include "SceneData/IntersectionResult.h"

class Material {
public:
	Material() {
		
	}
	virtual ~Material() {
		
	}
	
	virtual Color GetAmbientColor(const IntersectionResult &intersectionResult) const = 0;
	virtual Color GetDirectColor(const IntersectionResult &intersectionResult) const = 0;
	virtual Color GetColorForAreaLight(const IntersectionResult& intersectionResult) const = 0;
};
