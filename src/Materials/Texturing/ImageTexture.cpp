#include "Materials/Texturing/ImageTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/stb/stb_image.h"
#include "ThirdParty/stb/stb_image_write.h"
#include <sstream>
#include <iostream>

// TODO: separate texture data from texture representation
// TODO: have a texture instance block in json file
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
	
	// the resulting texture will be divided by two multiple times until
	// the lesser dimension gets to be 1 pixel
	std::vector<int> widthSizes, heightSizes;
	int totalNumberOfPixels = 0;
	int currWidth = texWidth;
	int currHeight = texHeight;
	while (currWidth >= 1 && currHeight >= 1) {
		totalNumberOfPixels += currWidth*currHeight;
		widthSizes.push_back(currWidth);
		heightSizes.push_back(currHeight);
		currWidth /= 2;
		currHeight /= 2;
	}
	
	float *mipMapLevels = new float[totalNumberOfPixels*texChannels];
	// copy mip level 0
	memcpy(mipMapLevels, pixels, texWidth*texHeight*texChannels*sizeof(float));
	float *oldMipLevel, *newMipLevel;
	int offsetBaseMip = 0;
	float sumNormalization = 0.25f;
	for (int i = 1; i < widthSizes.size(); i++) {
		// prev dimensions will always be longer
		int prevWidth = widthSizes[i - 1];
		int prevHeight = heightSizes[i - 1];
		
		int currWidth = widthSizes[i];
		int currHeight = heightSizes[i];
		
		oldMipLevel = &pixels[offsetBaseMip];
		newMipLevel = &pixels[offsetBaseMip + prevWidth*prevHeight*texChannels];
		
		for (int row = 0, pixel = 0; row < currHeight; row++) {
			for (int col = 0; col < currWidth; col++, pixel += texChannels) {
				// subsample from previous resolution
				int sourceRow = row * 2;
				int sourceCol = col * 2;
				// TODO: make sure this doesn't happen
				if (sourceRow > prevHeight - 1) {
					sourceRow = prevHeight - 1;
				}
				if (sourceCol > prevWidth - 1) {
					sourceCol = prevWidth - 1;
				}
				int sourceRowOffset = sourceRow * prevWidth * texChannels;
				int sourcePixel1 = sourceRowOffset + sourceCol;
				// one column over
				int sourcePixel2 = sourcePixel2 + texChannels;
				// next row
				int sourcePixel3 = sourceRowOffset + prevWidth * texChannels;
				// one column over
				int sourcePixel4 = sourcePixel3 + texChannels;
				// simple box filter
				for (int channel = 0; channel < texChannels; channel++) {
					newMipLevel[pixel + channel] =
					(oldMipLevel[sourcePixel1 + channel] + oldMipLevel[sourcePixel2 + channel]
					+ oldMipLevel[sourcePixel3 + channel] + oldMipLevel[sourcePixel4 + channel])*
					sumNormalization;
				}
			}
		}
	}
	
	delete [] mipMapLevels;
}

