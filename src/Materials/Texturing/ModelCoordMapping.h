#pragma once

#include "Materials/Texturing/MappingLayer.h"

class ModelCoordMapping : public MappingLayer {
public:
	void ComputeTextureCoordinates(ShadingInfo const & shadingInfo,
								   int width, int height,
								   int & row, int & column) {
		row = (int)height*shadingInfo.u;
		column = (int)width*shadingInfo.v;
	}
};
