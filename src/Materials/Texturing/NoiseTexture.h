#pragma once

#include "Materials/Texturing/AbstractTexture.h"
#include "Materials/Texturing/Mapping/NullMapping.h"
#include "Materials/Color3.h"
#include "Math/CubicNoiseFunction.h"
#include <memory>
#include <cmath>

class NoiseTexture : public AbstractTexture {
public:
	NoiseTexture(Color3 const & minColor,
				 Color3 const & maxColor,
				 std::shared_ptr<NoiseFunction> const & noiseFunction,
				 bool useWrapping,
				 float expansionNumber)
	: AbstractTexture(std::make_shared<NullMapping>(), SamplingType::Nearest),
		noiseFunction(noiseFunction), minColor(minColor), maxColor(maxColor) {
		GetColorFunc = useWrapping ? &NoiseTexture::GetColorWrapping :
			&NoiseTexture::GetColorNoWrapping;
	}
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const override {
		return (this->*GetColorFunc)(shadingInfo);
	}
	
private:
	Color3 (NoiseTexture::*GetColorFunc)(const ShadingInfo&) const;
	
	// ideally FBM requires a proper noise func
	Color3 GetColorNoWrapping(const ShadingInfo& shadingInfo) const {
		Point3 const & localPoint = shadingInfo.intersectionPositionLocal;
		float noiseValue = expansionNumber * noiseFunction->GetValueFBM(localPoint);
		float value = noiseValue - floor(noiseValue);
		
		return minColor * value;
	}
	
	Color3 GetColorWrapping(const ShadingInfo& shadingInfo) const {
		Point3 const & localPoint = shadingInfo.intersectionPositionLocal;
		float noiseValue = noiseFunction->GetValueFBM(localPoint);
		return minColor + (maxColor - minColor) * noiseValue;
	}
	
	std::shared_ptr<NoiseFunction> noiseFunction;
	Color3 minColor, maxColor;
	float expansionNumber;
};
