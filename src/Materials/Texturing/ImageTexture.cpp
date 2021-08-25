#include "Materials/Texturing/ImageTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/stb/stb_image.h"
#include "ThirdParty/stb/stb_image_write.h"
#include <sstream>
#include <iostream>

ImageTexture::ImageTexture(std::shared_ptr<MappingLayer> const & mappingLayer,
						   std::string const & filePath)
	: AbstractTexture(mappingLayer) {
#if __APPLE__
	std::string scenePath = "../../" + filePath;
#else
	std::string scenePath = "../" + filePath;
#endif
		
	unsigned char *charPixels = stbi_load(scenePath.c_str(),
		&texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	if (charPixels == nullptr) {
		std::stringstream exceptionMsg;
		exceptionMsg << "Cannot open texture for reading: " <<
			scenePath.c_str() << ".\n";
		throw exceptionMsg;
	}
	else {
		std::cout << "Read texture " << scenePath << ", dim: ("
			<< texWidth << " x " << texHeight << "), channels: "
			<< texChannels << ".\n";
	}
	
	// we wanted alpha
	texChannels = 4;
	pixels = new float[texWidth*texHeight*texChannels];
	int numPixels = texWidth*texHeight*texChannels;
	rowStride = texWidth*texChannels;
	float normFactor = 1.0f/255.0f;
	for (int pixel = 0; pixel < numPixels; pixel++) {
		pixels[pixel] = (float)charPixels[pixel]*normFactor;
	}
	delete [] charPixels;
}

ImageTexture::~ImageTexture() {
	if (pixels != nullptr) {
		delete [] pixels;
	}
}

Color3 ImageTexture::GetColor(const ShadingInfo& shadingInfo) const {
	int row, column;
	mappingLayer->ComputeTextureCoordinates(shadingInfo, texWidth, texHeight,
											row, column);
	// If it is nested, we need a way to represent the local intersection point
	int pixelOffset = row*rowStride + column*texChannels;
	return Color3(pixels[pixelOffset], pixels[pixelOffset + 1],
				  pixels[pixelOffset + 2]);
}
