#pragma once

#include "Materials/Color3.h"
#include "SceneData/ShadingInfo.h"
#include "Materials/Texturing/Mapping/MappingLayer.h"
#include <memory>
#include <string>

class AbstractTexture {
public:
	enum class SamplingType {
		Nearest,
		Bilinear,
		Trilinear
	};
	
	AbstractTexture(std::shared_ptr<MappingLayer> const & mappingLayer,
					SamplingType samplingType,
					std::string const & name) :
		mappingLayer(mappingLayer), samplingType(samplingType),
		name(name) {
	}
	
	virtual ~AbstractTexture() {
	}
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const = 0;
	
	std::shared_ptr<MappingLayer> GetMappingLayer() const {
		return mappingLayer;
	}
	
	SamplingType GetSamplingType() const {
		return samplingType;
	}
	
	std::string GetName() const {
		return name;
	}
	
	void SetName(std::string const & newName) {
		name = newName;
	}
	
protected:
	std::shared_ptr<MappingLayer> mappingLayer;
	SamplingType samplingType;
	std::string name;
};
