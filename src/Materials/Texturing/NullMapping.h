#pragma once

#include "Materials/Texturing/NullMapping.h"

class NullMapping : public MappingLayer {
public:

	void ComputeTextureCoordinates(ShadingInfo const & shadingInfo,
								   int width, int height,
								   int & row, int & column) {
		row = 0;
		column = 0;
	}

};

