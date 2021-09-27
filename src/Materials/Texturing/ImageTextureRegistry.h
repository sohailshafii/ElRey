#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "Materials/Texturing/AbstractTexture.h"
#include "Materials/Texturing/ImageTexture.h"
#include "Materials/Texturing/TextureData.h"
#include "Materials/Texturing/Mapping/MappingLayer.h"

class ImageTextureRegistry {
public:
	ImageTextureRegistry() { }
	// enforce as a singleton
	ImageTextureRegistry(ImageTextureRegistry const &) = delete;
	ImageTextureRegistry& operator=
	(ImageTextureRegistry const &) = delete;
	
	static ImageTextureRegistry& GetInstance();
	
	std::shared_ptr<ImageTexture> GetTextureForPath(std::string const & filePath,
		std::shared_ptr<MappingLayer> const & mappingLayer,
		AbstractTexture::SamplingType sampleType);
	
	// TODO: separate texture file loading from actual texture object
private:
	std::unordered_map<std::string, std::shared_ptr<TextureData>>
		pathToData;
	std::unordered_map<std::string, std::shared_ptr<ImageTexture>>
		pathToTexture;
};
