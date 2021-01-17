#pragma once

#include "Materials/Color.h"
#include "SceneData/ShadingInfo.h"

class Material {
public:
	Material() {
		
	}
	virtual ~Material() {
		
	}
	
	virtual Color GetAmbientColor(const ShadingInfo &shadingInfo) const = 0;
	virtual Color GetDirectColor(const ShadingInfo &shadingInfo) const = 0;
	virtual Color GetColorForAreaLight(const ShadingInfo &shadingInfo) const = 0;
};
