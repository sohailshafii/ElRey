#pragma once

#include "Materials/Texturing/MappingLayer.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"
#include <cmath>

class RectangularMapping : public MappingLayer {
public:
	RectangularMapping(float iRecWidth, float iRecHeight, unsigned int iWidthAxis,
					   unsigned int iHeightAxis, Point3 const & iOrigin) :
		recWidth(iRecWidth), recHeight(iRecHeight),
		recWidthInv(1.0f/iRecWidth), recHeightInv(1.0f/iRecHeight),
		widthAxis(iWidthAxis), heightAxis(iHeightAxis),
		origin(iOrigin) { }
	
	virtual void ComputeTextureCoordinates(ShadingInfo const & shadingInfo,
										   int width, int height,
										   int & row, int & column) override {
		auto intersectionPntLocal = shadingInfo.intersectionPositionLocal;
		intersectionPntLocal = invTransformMatrix * intersectionPntLocal;
		// if bigger than rec width or height, tile it
		Vector3 posOffsetFromOrigin = origin - intersectionPntLocal;
		float widthPos = posOffsetFromOrigin[widthAxis];
		float heightPos = posOffsetFromOrigin[heightAxis];
		
		widthPos = fabs(fmod(widthPos, recWidth));
		heightPos = fabs(fmod(heightPos, recHeight));
		// now normalize
		widthPos *= recWidthInv;
		heightPos *= recHeightInv;
		// remember that row is flipped in image space
		row = (height - 1) - (int)(widthPos * (height - 1));
		column = (int)(heightPos * (width - 1));
		CommonMath::Clamp(column, 0, width - 1);
		CommonMath::Clamp(row, 0, height - 1);
	}
	
private:
	float recWidth;
	float recHeight;
	float recWidthInv;
	float recHeightInv;
	
	// 0, 1 or 2
	unsigned int widthAxis;
	unsigned int heightAxis;
	
	Point3 origin;
};

