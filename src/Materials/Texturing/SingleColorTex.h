#pragma once

#include "Materials/Texturing/AbstractTexture.h"
#include "Materials/Texturing/NullMapping.h"

class SingleColorTex : public AbstractTexture {
public:
	SingleColorTex(std::shared_ptr<MappingLayer> const & mappingLayer,
				   Color3 const & color) : AbstractTexture(mappingLayer),
		constantColor(color) {
	}
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const override {
		return constantColor;
	}
	
	static SingleColorTex SolidWhite() {
		return SingleColorTex(std::make_shared<NullMapping>(), Color3::White());
	}
	
	virtual Color3 GetColor(int row, int column, float rowFloat, float colFloat) const override {
		return constantColor;
	}

private:
	Color3 constantColor;
};
