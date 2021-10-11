#pragma once

#include "Materials/Texturing/AbstractTexture.h"
#include "Materials/Texturing/TextureData.h"
#include "Materials/Texturing/Mapping/NullMapping.h"
#include "Materials/Color3.h"
#include "Math/NoiseFunction.h"
#include <memory>
#include <cmath>

class NoiseRampTexture : public AbstractTexture {
public:
	NoiseRampTexture(std::shared_ptr<TextureData> const & rampTexture,
				 std::shared_ptr<NoiseFunction> const & noiseFunction,
				 float a)
	: AbstractTexture(std::make_shared<NullMapping>(), SamplingType::Nearest),
		rampTexture(rampTexture), noiseFunction(noiseFunction), a(a) {
	}
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const override {
		Point3 const & localPoint = shadingInfo.intersectionPositionLocal;
		float noiseValue = noiseFunction->GetValue(localPoint);
		float y = localPoint[1] + a * noiseValue;
		float u = 0.5 * (1.0f + sin(y));
		int row = 0;
		int column = u * rampTexture->texWidth;
		float *pixelOffset = &rampTexture->pixels[column*rampTexture->texChannels];
		return Color3(pixelOffset[0], pixelOffset[1], pixelOffset[2]);
	}
	
private:
	std::shared_ptr<TextureData> rampTexture;
	std::shared_ptr<NoiseFunction> noiseFunction;
	float a;
};
