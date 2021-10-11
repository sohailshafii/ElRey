#pragma once

#include "Materials/Texturing/AbstractTexture.h"
#include "Materials/Texturing/Mapping/NullMapping.h"
#include "Materials/Color3.h"
#include "Math/CubicNoiseFunction.h"
#include <memory>

class CubicNoiseTexture : public AbstractTexture {
public:
	CubicNoiseTexture(unsigned int numOctaves,
					float lacuranity,
					float gain,
					Color3 const & minColor,
					Color3 const & maxColor)
	: AbstractTexture(std::make_shared<NullMapping>(), SamplingType::Nearest),
	minColor(minColor), maxColor(maxColor){
		cubicNoiseFunction =
		std::make_shared<CubicNoiseFunction>(numOctaves, lacuranity, gain);
	}
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const override {
		Point3 const & localPoint = shadingInfo.intersectionPositionLocal;
		float noiseValue = cubicNoiseFunction->GetValueInterpolated(localPoint);
		return minColor + (maxColor - minColor) * noiseValue;
	}
	
private:
	std::shared_ptr<CubicNoiseFunction> cubicNoiseFunction;
	Color3 minColor, maxColor;
};
