#pragma once

#include "Materials/Texturing/AbstractTexture.h"

class SingleColorTex : public AbstractTexture {
public:
	SingleColorTex(Color3 color) : constantColor(color) {
	}
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const override {
		return constantColor;
	}
	
	static SingleColorTex SolidWhite() {
		return SingleColorTex(Color3::White());
	}

private:
	Color3 constantColor;
};
