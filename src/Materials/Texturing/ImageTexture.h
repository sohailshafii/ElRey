#pragma once

#include "Materials/Texturing/AbstractTexture.h"
#include "Materials/Texturing/MappingLayer.h"
#include <string>

class ImageTexture : public AbstractTexture {
public:
	ImageTexture(std::shared_ptr<MappingLayer> const & mappingLayer,
				 std::string const & filePath);
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const override;

private:
};

