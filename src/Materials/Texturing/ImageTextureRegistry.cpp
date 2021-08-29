#include "ImageTextureRegistry.h"

ImageTextureRegistry& ImageTextureRegistry::GetInstance() {
	static ImageTextureRegistry instance;
	return instance;
}

std::shared_ptr<ImageTexture> ImageTextureRegistry::GetTextureForPath(std::string const & filePath,
												std::shared_ptr<MappingLayer> const & mappingLayer,
												AbstractTexture::SamplingType sampleType) {
	if (pathToTexture.find(filePath) != pathToTexture.end()) {
		return pathToTexture[filePath];
	}
	auto createdTex = std::make_shared<ImageTexture>(mappingLayer, filePath, sampleType);
	pathToTexture[filePath] = createdTex;
	return createdTex;
}
