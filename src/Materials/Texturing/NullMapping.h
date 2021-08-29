#pragma once

#include "Materials/Texturing/MappingLayer.h"

class NullMapping : public MappingLayer {
public:
	NullMapping() : MappingLayer(MappingLayer::WrapType::Clamp, false) {
	}
	
	virtual void ComputeTextureCoordinates(ShadingInfo const & shadingInfo,
										   int width, int height,
										   float & row, float & column) override {
		row = 0;
		column = 0;
	}

};

