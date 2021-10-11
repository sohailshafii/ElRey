#pragma once

#include "Materials/Texturing/AbstractTexture.h"
#include "Materials/Texturing/Mapping/NullMapping.h"
#include "Materials/Color3.h"
#include "Math/LinearNoiseFunction.h"
#include <memory>

class LinearNoiseTexture : public AbstractTexture {
public:
	LinearNoiseTexture(unsigned int numOctaves,
					float lacuranity,
					float gain,
					Color3 const & minColor,
					Color3 const & maxColor)
	: AbstractTexture(std::make_shared<NullMapping>(), SamplingType::Nearest),
	minColor(minColor), maxColor(maxColor){
		linearNoiseFunction =
		std::make_shared<LinearNoiseFunction>(numOctaves, lacuranity, gain);
	}
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const override {
		Point3 const & localPoint = shadingInfo.intersectionPositionLocal;
		float noiseValue = linearNoiseFunction->GetValue(localPoint);
		Color3 colorRange = maxColor - minColor;
		return minColor + colorRange * noiseValue;
	}
	
private:
	std::shared_ptr<LinearNoiseFunction> linearNoiseFunction;
	Color3 minColor, maxColor;
};
