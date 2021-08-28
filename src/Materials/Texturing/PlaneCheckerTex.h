#pragma once

#include "Materials/Texturing/AbstractTexture.h"
#include "Materials/Texturing/NullMapping.h"
#include "Materials/Color3.h"

class PlaneCheckerTex : public AbstractTexture {
public:
	PlaneCheckerTex(unsigned int checkerSize, unsigned int outlineWidth,
					Color3 const & inColor, Color3 const & outColor,
					Color3 const & outlineColor)
		: AbstractTexture(std::make_shared<NullMapping>()), checkerSize(checkerSize),
			inColor(inColor), outColor(outColor), outlineColor(outlineColor),
			outlineWidth(outlineWidth) {
			outlineWidthHalfNormalized = 0.5f * outlineWidth / checkerSize;
			outlineWidthHalfNormalizedUpper = 1.0f - outlineWidthHalfNormalized;
	}
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const override {
		Point3 transformedPoint = mappingLayer->GetTransformedTexturePnt(shadingInfo.intersectionPositionLocal,
											shadingInfo.intersectionPosition);
		float x = transformedPoint[0];
		float z = transformedPoint[2];
		int cellX = floor(x / checkerSize);
		int cellZ = floor(z / checkerSize);
		return GetColor(cellZ, cellX, z, x);
	}

protected:
	
	virtual Color3 GetColor(int row, int column, float rowFloat, float colFloat) const override {
		float fractionX = colFloat / checkerSize - column;
		float fractionZ = rowFloat / checkerSize - row;
		bool inOutline = (fractionX < outlineWidthHalfNormalized ||
						  fractionX > outlineWidthHalfNormalizedUpper) ||
						 (fractionZ < outlineWidthHalfNormalized ||
						  fractionZ > outlineWidthHalfNormalizedUpper);
		if (inOutline) {
			return outlineColor;
		}
		else if ((column + row) % 2 == 0) {
			return inColor;
		}
		else {
			return outColor;
		}
	}
	
private:
	unsigned int checkerSize;
	unsigned int outlineWidth;
	Color3 inColor, outColor, outlineColor;
	float outlineWidthHalfNormalized, outlineWidthHalfNormalizedUpper;
};
