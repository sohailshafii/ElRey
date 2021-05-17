#include "Materials/Texturing/ImageTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/stb/stb_image.h"

ImageTexture::ImageTexture(std::shared_ptr<MappingLayer> const & mappingLayer,
						   std::string const & filePath)
	: AbstractTexture(mappingLayer) {
	int texWidth, texHeight, texChannels;
	unsigned char* pixels = stbi_load(filePath.c_str(),
		&texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
}

Color3 ImageTexture::GetColor(const ShadingInfo& shadingInfo) const {
	return Color3::White();
}
