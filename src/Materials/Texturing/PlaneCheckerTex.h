#pragma once

#include "Materials/Texturing/AbstractTexture.h"
#include "Materials/Texturing/Mapping/NullMapping.h"
#include "Materials/Color3.h"

class PlaneCheckerTex : public AbstractTexture {
public:
	PlaneCheckerTex(unsigned int checkerSize, unsigned int outlineWidth,
					Color3 const & inColor, Color3 const & outColor,
					Color3 const & outlineColor)
		: AbstractTexture(std::make_shared<NullMapping>(), SamplingType::Nearest),
			checkerSize(checkerSize), inColor(inColor), outColor(outColor), outlineColor(outlineColor),
			outlineWidth(outlineWidth) {
			outlineWidthHalfNormalized = 0.5f * outlineWidth / checkerSize;
			outlineWidthHalfNormalizedUpper = 1.0f - outlineWidthHalfNormalized;
	}
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const override {
		Point3 transformedPoint = mappingLayer->GetTransformedTexturePnt(shadingInfo.intersectionPositionLocal,
											shadingInfo.intersectionPosition);
		float x = transformedPoint[0];
		float z = transformedPoint[2];
		int column = floor(x / checkerSize);
		int row = floor(z / checkerSize);
		float fractionCol = x / checkerSize - column;
		float fractionRow = z / checkerSize - row;
		
		bool inOutline = (fractionRow < outlineWidthHalfNormalized ||
						  fractionRow > outlineWidthHalfNormalizedUpper) ||
						 (fractionCol < outlineWidthHalfNormalized ||
						  fractionCol > outlineWidthHalfNormalizedUpper);
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
