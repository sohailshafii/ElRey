#pragma once

#include "Materials/Texturing/MappingLayer.h"

class RectangularMapping : public MappingLayer {
public:
	virtual void ComputeTextureCoordinates(ShadingInfo const & shadingInfo,
										   int width, int height,
										   int & row, int & column) override {
		// bias and shift to [0, 1]
		float zPos = (shadingInfo.intersectionPositionLocal[2] + 1.0f)/2.0f;
		float xPos = (shadingInfo.intersectionPositionLocal[0] + 1.0f)/2.0f;
		row = (int)(zPos * (width - 1));
		column = (int)(xPos * (height - 1));
	}
};

