#pragma once

#include "Materials/Texturing/AbstractTexture.h"
#include "Materials/Texturing/MappingLayer.h"
#include <string>

class ImageTexture : public AbstractTexture {
public:
	ImageTexture(std::shared_ptr<MappingLayer> const & mappingLayer,
				 std::string const & filePath);
	
	~ImageTexture();
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const override;

protected:
	
	virtual Color3 GetColor(int row, int column, float rowFloat, float colFloat) const override {
		// If it is nested, we need a way to represent the local intersection point
		int pixelOffset = row*rowStride + column*texChannels;
		return Color3(pixels[pixelOffset], pixels[pixelOffset + 1],
					  pixels[pixelOffset + 2]);
	}

private:
	float *pixels;
	int texWidth, texHeight;
	int texChannels, rowStride;
	int stride;
};

