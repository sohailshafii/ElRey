#pragma once

#include "Materials/Texturing/MappingLayer.h"

class NullMapping : public MappingLayer {
public:
	NullMapping() : MappingLayer(MappingLayer::WrapType::Clamp, false) {
	}
	
	void ComputeTextureCoordinates(ShadingInfo const & shadingInfo,
								   int width, int height,
								   int & row, int & column) {
		row = 0;
		column = 0;
	}

};

