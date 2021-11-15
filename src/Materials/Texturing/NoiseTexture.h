#pragma once

#include "Materials/Texturing/AbstractTexture.h"
#include "Materials/Texturing/Mapping/NullMapping.h"
#include "Materials/Color3.h"
#include "Math/CubicNoiseFunction.h"
#include <memory>
#include <cmath>

class NoiseTexture : public AbstractTexture {
public:
	enum class FunctionType {
		ValueInterp = 0, VectorInterp,
		ValueFBM, VectorFBM, ValueTurbFBM,
		ValueFractalSum, VectorFractalSum
	};
	
	enum class ColorType {
		Wrapping = 0, NoWrapping, Vector
	};
	
	NoiseTexture(Color3 const & minColor,
				 Color3 const & maxColor,
				 std::shared_ptr<NoiseFunction> const & noiseFunctionClass,
				 ColorType colorType,
				 float expansionNumber,
				 FunctionType functionType)
	: AbstractTexture(std::make_shared<NullMapping>(), SamplingType::Nearest),
		noiseFunction(noiseFunctionClass), minColor(minColor), maxColor(maxColor) {
		switch(colorType) {
			case ColorType::Wrapping:
				GetColorFunc = &NoiseTexture::GetColorWrapping;
				break;
			case ColorType::NoWrapping:
				GetColorFunc = &NoiseTexture::GetColorNoWrapping;
				break;
			case ColorType::Vector:
				GetColorFunc = &NoiseTexture::GetColorVector;
				break;
		}
		switch(functionType) {
			case FunctionType::ValueInterp:
				GetNoiseFunc = &NoiseTexture::GetNoiseInterp;
				break;
			case FunctionType::VectorInterp:
				GetNoiseFunc = &NoiseTexture::GetNoiseVectorInterp;
				break;
			case FunctionType::ValueFBM:
				GetNoiseFunc = &NoiseTexture::GetNoiseValueFBM;
				break;
			case FunctionType::VectorFBM:
				GetNoiseFunc = &NoiseTexture::GetNoiseVectorFBM;
				break;
			case FunctionType::ValueTurbFBM:
				GetNoiseFunc = &NoiseTexture::GetNoiseValueTurbFBM;
				break;
			case FunctionType::ValueFractalSum:
				GetNoiseFunc = &NoiseTexture::GetNoiseValueFractalSum;
				break;
			case FunctionType::VectorFractalSum:
			default:
				GetNoiseFunc = &NoiseTexture::GetNoiseVectorFractalSum;
				break;
		}
	}
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const override {
		return (this->*GetColorFunc)(shadingInfo);
	}
	
private:
	Color3 (NoiseTexture::*GetColorFunc)(ShadingInfo const &) const;
	Color3 (NoiseTexture::*GetNoiseFunc)(ShadingInfo const &) const;
	
	// ideally FBM requires a proper noise func
	Color3 GetColorNoWrapping(ShadingInfo const & shadingInfo) const {
		Point3 const & localPoint = shadingInfo.intersectionPositionLocal;
		Color3 noiseColor = (this->*GetNoiseFunc)(shadingInfo);
		float noiseValue = expansionNumber * noiseColor[0];
		float value = noiseValue - floor(noiseValue);
		
		return minColor * value;
	}
	
	Color3 GetColorWrapping(ShadingInfo const & shadingInfo) const {
		Point3 const & localPoint = shadingInfo.intersectionPositionLocal;
		Color3 noiseColor = (this->*GetNoiseFunc)(shadingInfo);
		float noiseValue = noiseColor[0];
		return minColor + (maxColor - minColor) * noiseValue;
	}
	
	Color3 GetColorVector(ShadingInfo const & shadingInfo) const {
		Point3 const & localPoint = shadingInfo.intersectionPositionLocal;
		return (this->*GetNoiseFunc)(shadingInfo);
	}
		
	Color3 GetNoiseInterp(ShadingInfo const & shadingInfo) const {
		Point3 const & localPoint = shadingInfo.intersectionPositionLocal;
		float lerpVal = noiseFunction->GetValueInterpolated(localPoint);
		return Color3(lerpVal, lerpVal, lerpVal);
	}
	
	Color3 GetNoiseVectorInterp(ShadingInfo const & shadingInfo) const {
		Point3 const & localPoint = shadingInfo.intersectionPositionLocal;
		auto lerpVal = noiseFunction->GetVectorValueInterpolated(localPoint);
		return Color3(lerpVal[0], lerpVal[1], lerpVal[2]);
	}
	
	Color3 GetNoiseValueFBM(ShadingInfo const & shadingInfo) const {
		Point3 const & localPoint = shadingInfo.intersectionPositionLocal;
		float lerpVal = noiseFunction->GetValueFBM(localPoint);
		return Color3(lerpVal, lerpVal, lerpVal);
	}
	
	Color3 GetNoiseVectorFBM(ShadingInfo const & shadingInfo) const {
		Point3 const & localPoint = shadingInfo.intersectionPositionLocal;
		auto lerpVal = noiseFunction->GetVectorValueFBM(localPoint);
		return Color3(lerpVal[0], lerpVal[1], lerpVal[2]);
	}
	
	Color3 GetNoiseValueTurbFBM(ShadingInfo const & shadingInfo) const {
		Point3 const & localPoint = shadingInfo.intersectionPositionLocal;
		float lerpVal = noiseFunction->GetValueTurbulenceFBM(localPoint);
		return Color3(lerpVal, lerpVal, lerpVal);
	}
	
	Color3 GetNoiseValueFractalSum(ShadingInfo const & shadingInfo) const {
		Point3 const & localPoint = shadingInfo.intersectionPositionLocal;
		float lerpVal = noiseFunction->GetValueFractalSum(localPoint);
		return Color3(lerpVal, lerpVal, lerpVal);
	}
	
	Color3 GetNoiseVectorFractalSum(ShadingInfo const & shadingInfo) const {
		Point3 const & localPoint = shadingInfo.intersectionPositionLocal;
		auto lerpVal = noiseFunction->GetVectorFractalSum(localPoint);
		return Color3(lerpVal[0], lerpVal[1], lerpVal[2]);
	}
	
	std::shared_ptr<NoiseFunction> noiseFunction;
	Color3 minColor, maxColor;
	float expansionNumber;
};
