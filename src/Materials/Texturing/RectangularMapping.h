#pragma once

#include "Materials/Texturing/MappingLayer.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"
#include <cmath>

class RectangularMapping : public MappingLayer {
public:
	RectangularMapping(float iRecWidth, float iRecHeight, unsigned int iWidthAxis,
					   unsigned int iHeightAxis, Point3 const & iOrigin,
					   WrapType wrapType, bool useWorldCoordsForTex) :
		MappingLayer(wrapType, useWorldCoordsForTex), recWidth(iRecWidth), recHeight(iRecHeight),
		recWidthInv(1.0f/iRecWidth), recHeightInv(1.0f/iRecHeight),
		widthAxis(iWidthAxis), heightAxis(iHeightAxis),
		origin(iOrigin) { }
	
	virtual void ComputeTextureCoordinates(ShadingInfo const & shadingInfo,
										   int width, int height,
										   float & row, float & column) override {
		auto intersectionPnt = useWorldCoordsForTex ?
			shadingInfo.intersectionPosition : shadingInfo.intersectionPositionLocal;
		intersectionPnt = invTransformMatrix * intersectionPnt;
		// if bigger than rec width or height, tile it
		Vector3 posOffsetFromOrigin = origin - intersectionPnt;
		float widthPos = posOffsetFromOrigin[widthAxis];
		float heightPos = posOffsetFromOrigin[heightAxis];
		
		widthPos = fabs(fmod(widthPos, recWidth));
		heightPos = fabs(fmod(heightPos, recHeight));
		// now normalize
		widthPos *= recWidthInv;
		heightPos *= recHeightInv;
		// remember that row is flipped in image space
		row = (float)(height - 1) - widthPos * (float)(height - 1);
		column = heightPos * (float)(width - 1);
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

