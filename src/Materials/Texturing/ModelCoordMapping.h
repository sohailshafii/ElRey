#pragma once

#include "Materials/Texturing/MappingLayer.h"

class ModelCoordMapping : public MappingLayer {
public:
	void ComputeTextureCoordinates(ShadingInfo const & shadingInfo,
								   int width, int height,
								   int & row, int & column) {
		Point3 uvCoord(shadingInfo.u, shadingInfo.v, 0.0f);
		uvCoord = invTransformMatrix * uvCoord;
		row = (int)height*shadingInfo.u;
		column = (int)width*shadingInfo.v;
		
		CommonMath::Clamp(column, 0, width - 1);
		CommonMath::Clamp(row, 0, height - 1);
	}
};
