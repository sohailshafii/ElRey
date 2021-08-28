#pragma once

#include "Materials/Texturing/MappingLayer.h"

class ModelCoordMapping : public MappingLayer {
public:
	ModelCoordMapping(WrapType wrapType) : MappingLayer(wrapType, false) {
	}
	
	void ComputeTextureCoordinates(ShadingInfo const & shadingInfo,
								   int width, int height,
								   int & row, int & column) {
		Point3 uvCoord(shadingInfo.u, shadingInfo.v, 0.0f);
		uvCoord = invTransformMatrix * uvCoord;
		row = (int)height*shadingInfo.u;
		column = (int)width*shadingInfo.v;
		
		(*wrapFunction)(column, width - 1);
		(*wrapFunction)(row, height - 1);
	}
};
