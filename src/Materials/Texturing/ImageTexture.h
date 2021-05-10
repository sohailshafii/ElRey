#pragma once

#include "Materials/Texturing/AbstractTexture.h"
#include <string>

class ImageTexture : public AbstractTexture {
public:
	ImageTexture(std::string const & filePath);
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const override;

private:
};

