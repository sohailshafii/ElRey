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
		auto const & localHitPoint = shadingInfo.intersectionPositionLocal;
		float x = localHitPoint[0];
		float z = localHitPoint[2];
		int cellX = floor(x / checkerSize);
		int cellZ = floor(z / checkerSize);
		float fractionX = x / checkerSize - cellX;
		float fractionZ = z / checkerSize - cellZ;
		bool inOutline = (fractionX < outlineWidthHalfNormalized ||
						  fractionX > outlineWidthHalfNormalizedUpper) ||
						 (fractionZ < outlineWidthHalfNormalized ||
						  fractionZ > outlineWidthHalfNormalizedUpper);
		if (inOutline) {
			return outlineColor;
		}
		else if ((cellX + cellZ) % 2 == 0) {
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
