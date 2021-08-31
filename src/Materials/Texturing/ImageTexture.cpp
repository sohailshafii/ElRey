#include "Materials/Texturing/ImageTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/stb/stb_image.h"
#include "ThirdParty/stb/stb_image_write.h"
#include <sstream>
#include <iostream>

ImageTexture::ImageTexture(std::shared_ptr<MappingLayer> const & mappingLayer,
						   std::string const & filePath,
						   SamplingType samplingType)
	: AbstractTexture(mappingLayer, samplingType) {
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
		
	sampleFunction = samplingType == SamplingType::Nearest ?
		&ImageTexture::SampleNearest : &ImageTexture::SampleBilinear;
}

ImageTexture::~ImageTexture() {
	if (pixels != nullptr) {
		delete [] pixels;
	}
}

Color3 ImageTexture::GetColor(const ShadingInfo& shadingInfo) const {
	float row, column;
	mappingLayer->ComputeTextureCoordinates(shadingInfo, texWidth, texHeight,
											row, column);
	return (this->*sampleFunction)(row, column, texWidth, texHeight);
}

void ImageTexture::ComputeMipmaps() {
	// make sure texture's dimensions are both powers of two
	if (!CommonMath::IsPowerOfTwo(texWidth)) {
		std::stringstream exceptionMsg;
		exceptionMsg << "Texture width " << texWidth << " not power of two.\n";
		throw exceptionMsg;
	}
	
	if (!CommonMath::IsPowerOfTwo(texHeight)) {
		std::stringstream exceptionMsg;
		exceptionMsg << "Texture width " << texHeight << " not power of two.\n";
		throw exceptionMsg;
	}
	
	// find out how many times you have to divide by two to get to smallest texture
	int pwrTwoWidth = CommonMath::FindPowerOfTwo(texWidth);
	int pwrTwoHeight = CommonMath::FindPowerOfTwo(texHeight);
	int pwrOfTwo = pwrTwoHeight < pwrTwoWidth ? pwrTwoHeight : pwrTwoWidth;
	
	/*std::stringstream exceptionMsg;
	exceptionMsg << "Could not find key: " << key
		<< " in JSON object: " << jsonObj << ".\n";
	throw exceptionMsg;*/
}

