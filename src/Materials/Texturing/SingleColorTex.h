#pragma once

#include "Materials/Texturing/AbstractTexture.h"
#include "Materials/Texturing/Mapping/NullMapping.h"

class SingleColorTex : public AbstractTexture {
public:
	SingleColorTex(std::shared_ptr<MappingLayer> const & mappingLayer,
				   Color3 const & color,
				   SamplingType samplingType,
				   std::string const & name) :
		AbstractTexture(mappingLayer, samplingType, name),
		constantColor(color) {
	}
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const override {
		return constantColor;
	}
	
	static SingleColorTex SolidWhite() {
		static int nameCounter = 0;
		return SingleColorTex(std::make_shared<NullMapping>(), Color3::White(),
							  SamplingType::Nearest,
							  "SolidWhite-"+std::to_string(nameCounter));
	}

private:
	Color3 constantColor;
};
