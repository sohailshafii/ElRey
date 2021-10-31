#include "ImageTextureRegistry.h"

ImageTextureRegistry& ImageTextureRegistry::GetInstance() {
	static ImageTextureRegistry instance;
	return instance;
}

std::shared_ptr<ImageTexture> ImageTextureRegistry::GetTextureForPath(std::string const & filePath,
												std::shared_ptr<MappingLayer> const & mappingLayer,
												AbstractTexture::SamplingType sampleType) {
	if (pathToTexture.find(filePath) != pathToTexture.end()) {
		auto foundTexture = pathToTexture[filePath];
		return pathToTexture[filePath];
	}
	std::shared_ptr<TextureData> textureData = GetTextureData(filePath);
	auto createdTex = std::make_shared<ImageTexture>(mappingLayer, textureData, sampleType);
	pathToTexture[filePath] = createdTex;
	return createdTex;
}

std::shared_ptr<TextureData> ImageTextureRegistry::GetTextureData(std::string const & filePath) {
	std::shared_ptr<TextureData> textureData;
	if (pathToData.find(filePath) != pathToData.end()) {
		textureData = pathToData[filePath];
	}
	else {
		textureData = std::make_shared<TextureData>(filePath);
	}
	
	return textureData;
}
