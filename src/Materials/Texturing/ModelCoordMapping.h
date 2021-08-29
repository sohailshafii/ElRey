#pragma once

#include "Materials/Texturing/MappingLayer.h"

class ModelCoordMapping : public MappingLayer {
public:
	ModelCoordMapping(WrapType wrapType) : MappingLayer(wrapType, false) {
	}
	
	virtual void ComputeTextureCoordinates(ShadingInfo const & shadingInfo,
								   int width, int height,
								   float & row, float & column) override {
		Point3 uvCoord(shadingInfo.u, shadingInfo.v, 0.0f);
		uvCoord = invTransformMatrix * uvCoord;
		row = height*shadingInfo.u;
		column = width*shadingInfo.v;
	}
};
