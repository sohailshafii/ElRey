#pragma once

#include "Materials/Color3.h"
#include "SceneData/ShadingInfo.h"
#include "Materials/Texturing/MappingLayer.h"
#include <memory>

class AbstractTexture {
public:
	enum class SamplingType {
		Nearest,
		Bilinear
	};
	
	AbstractTexture(std::shared_ptr<MappingLayer> const & mappingLayer,
					SamplingType samplingType) :
		mappingLayer(mappingLayer), samplingType(samplingType) {
	}
	
	virtual ~AbstractTexture() {
	}
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const = 0;
	
protected:
	std::shared_ptr<MappingLayer> mappingLayer;
	SamplingType samplingType;
};
