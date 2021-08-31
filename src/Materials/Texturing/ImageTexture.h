#pragma once

#include "Materials/Texturing/AbstractTexture.h"
#include "Materials/Texturing/Mapping/MappingLayer.h"
#include <string>

class ImageTexture : public AbstractTexture {
public:
	ImageTexture(std::shared_ptr<MappingLayer> const & mappingLayer,
				 std::string const & filePath,
				 SamplingType samplingType);
	
	~ImageTexture();
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const override;

private:
	float *pixels;
	int texWidth, texHeight;
	int texChannels, rowStride;
	int stride;
	
	Color3 (ImageTexture::*sampleFunction)(float, float, int, int) const;
	
	Color3 SampleNearest(float row, float col, int width, int height) const {
		int rowInt = (int)row;
		int colInt = (int)col;
		mappingLayer->ApplyWrap(colInt, width - 1);
		mappingLayer->ApplyWrap(rowInt, height - 1);
		return GetColor(rowInt, colInt);
	}

	Color3 SampleBilinear(float row, float col, int width, int height) const {
		int rowInt = (int)row;
		int colInt = (int)col;
		// next sample positions
		int nextRowInt = rowInt + 1;
		int nextColInt = colInt + 1;
		
		mappingLayer->ApplyWrap(colInt, width - 1);
		mappingLayer->ApplyWrap(rowInt, height - 1);
		mappingLayer->ApplyWrap(nextColInt, width - 1);
		mappingLayer->ApplyWrap(nextRowInt, height - 1);
		
		float fracRow = row - rowInt;
		float fracCol = col - colInt;
		
		Color3 sample00 = GetColor(rowInt, colInt);
		Color3 sample01 = GetColor(nextRowInt, colInt);
		Color3 sample10 = GetColor(rowInt, nextColInt);
		Color3 sample11 = GetColor(nextRowInt, nextColInt);

		float invFracCol = 1.0f - fracCol;
		Color3 lerpX1 = sample00 * invFracCol + sample10 * fracCol;
		Color3 lerpX2 = sample01 * invFracCol + sample11 * fracCol;
		
		float invFracRow = 1.0f - fracRow;
		return lerpX1 * invFracRow + lerpX2 * fracRow;
	}
	
	Color3 GetColor(int row, int col) const {
		int pixelOffset = row*rowStride + col*texChannels;
		return Color3(pixels[pixelOffset], pixels[pixelOffset + 1],
					  pixels[pixelOffset + 2]);
	}
	
	void ComputeMipmaps();
};

