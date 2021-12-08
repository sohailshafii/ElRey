#include "Materials/Texturing/ImageTexture.h"
#include <sstream>
#include <iostream>

ImageTexture::ImageTexture(std::shared_ptr<MappingLayer> const & mappingLayer,
						   std::shared_ptr<TextureData> const & textureData,
						   SamplingType samplingType,
						   std::string const & name)
	: AbstractTexture(mappingLayer, samplingType, name), textureData(textureData) {
	sampleFunction = samplingType == SamplingType::Nearest ?
		&ImageTexture::SampleNearest : &ImageTexture::SampleBilinear;
		
	// TODO: write sampler function for trilinear, not complete yet
	// see
	// https://gamedev.stackexchange.com/questions/130888/what-are-screen-space-derivatives-and-when-would-i-use-them
		// https://computergraphics.stackexchange.com/questions/5556/how-are-mipmap-levels-computed-in-metal
	// https://www.khronos.org/registry/OpenGL/extensions/OES/OES_standard_derivatives.txt
	// https://www.reddit.com/r/opengl/comments/3cdg5r/derivation_of_opengls_mipmap_level_computation/
		// need to compute ray differentials
	if (samplingType == SamplingType::Trilinear) {
		ComputeMipmaps();
	}
}

Color3 ImageTexture::GetColor(const ShadingInfo& shadingInfo) const {
	float row, column;
	mappingLayer->ComputeTextureCoordinates(shadingInfo, textureData->texWidth,
											textureData->texHeight,
											row, column);
	return (this->*sampleFunction)(row, column, textureData->texWidth, textureData->texHeight);
}

void ImageTexture::ComputeMipmaps() {
	// make sure texture's dimensions are both powers of two
	if (!CommonMath::IsPowerOfTwo(textureData->texWidth)) {
		std::stringstream exceptionMsg;
		exceptionMsg << "Texture width " << textureData->texWidth << " not power of two.\n";
		throw exceptionMsg;
	}
	
	if (!CommonMath::IsPowerOfTwo(textureData->texHeight)) {
		std::stringstream exceptionMsg;
		exceptionMsg << "Texture width " << textureData->texHeight << " not power of two.\n";
		throw exceptionMsg;
	}
	
	// find out how many times you have to divide by two to get to smallest texture
	int pwrTwoWidth = CommonMath::FindPowerOfTwo(textureData->texWidth);
	int pwrTwoHeight = CommonMath::FindPowerOfTwo(textureData->texHeight);
	int pwrOfTwo = pwrTwoHeight < pwrTwoWidth ? pwrTwoHeight : pwrTwoWidth;
	
	// the resulting texture will be divided by two multiple times until
	// the lesser dimension gets to be 1 pixel
	std::vector<int> widthSizes, heightSizes;
	int totalNumberOfPixels = 0;
	int currWidth = textureData->texWidth;
	int currHeight = textureData->texHeight;
	while (currWidth >= 1 && currHeight >= 1) {
		totalNumberOfPixels += currWidth*currHeight;
		widthSizes.push_back(currWidth);
		heightSizes.push_back(currHeight);
		currWidth /= 2;
		currHeight /= 2;
	}
	
	float *mipMapLevels = new float[totalNumberOfPixels*textureData->texChannels];
	// copy mip level 0
	memcpy(mipMapLevels, textureData->pixels,
		   textureData->texWidth*textureData->texHeight*textureData->texChannels*sizeof(float));
	float *oldMipLevel, *newMipLevel;
	int offsetBaseMip = 0;
	float sumNormalization = 0.25f;
	for (int i = 1; i < widthSizes.size(); i++) {
		// prev dimensions will always be longer
		int prevWidth = widthSizes[i - 1];
		int prevHeight = heightSizes[i - 1];
		
		int currWidth = widthSizes[i];
		int currHeight = heightSizes[i];
		
		oldMipLevel = &mipMapLevels[offsetBaseMip];
		newMipLevel = &mipMapLevels[offsetBaseMip +
									prevWidth*prevHeight*textureData->texChannels];
		int sourceRowWidth = prevWidth * textureData->texChannels;
		
		for (int row = 0, pixel = 0; row < currHeight; row++) {
			for (int col = 0; col < currWidth; col++, pixel += textureData->texChannels) {
				// subsample from previous resolution
				int sourceRow = row * 2;
				int sourceCol = col * 2;
				assert(sourceRow < prevHeight);
				assert(sourceCol < prevWidth);
				int sourceRowOffset = sourceRow * sourceRowWidth;
				int sourcePixel1 = sourceRowOffset + sourceCol;
				// one column over
				int sourcePixel2 = sourcePixel1 + textureData->texChannels;
				// gets items on next row
				int sourcePixel3 = sourcePixel1 + sourceRowWidth;
				// one column over
				int sourcePixel4 = sourcePixel3 + textureData->texChannels;
				// simple box filter
				for (int channel = 0; channel < textureData->texChannels; channel++) {
					newMipLevel[pixel + channel] = (oldMipLevel[sourcePixel1 + channel] +
													oldMipLevel[sourcePixel2 + channel] +
													oldMipLevel[sourcePixel3 + channel] +
													oldMipLevel[sourcePixel4 + channel])*
													sumNormalization;
				}
			}
		}
		
		offsetBaseMip += prevWidth*prevHeight*textureData->texChannels;
	}
	
	delete [] textureData->pixels;
	textureData->pixels = mipMapLevels;
}

