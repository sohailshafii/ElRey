#pragma once

#include "Materials/Texturing/AbstractTexture.h"
#include "Materials/Texturing/TextureData.h"
#include "Materials/Texturing/Mapping/MappingLayer.h"
#include <string>

class ImageTexture : public AbstractTexture {
public:
	ImageTexture(std::shared_ptr<MappingLayer> const & mappingLayer,
				 std::shared_ptr<TextureData> const & textureData,
				 SamplingType samplingType);
	
	bool operator==(ImageTexture const &other) const
	{
		return (mappingLayer == other.mappingLayer
			&& textureData == other.textureData
			&& samplingType == other.samplingType);
	}
	
	~ImageTexture() {
	}
	
	virtual Color3 GetColor(const ShadingInfo& shadingInfo) const override;
	
	std::shared_ptr<TextureData> GetTextureData() const {
		return textureData;
	}

private:
	std::shared_ptr<TextureData> const textureData;
	
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
		int pixelOffset = row*textureData->rowStride +
			col*textureData->texChannels;
		return Color3(textureData->pixels[pixelOffset],
					  textureData->pixels[pixelOffset + 1],
					  textureData->pixels[pixelOffset + 2]);
	}
	
	void ComputeMipmaps();
};

// https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
template <>
struct std::hash<ImageTexture>
{
	std::size_t operator()(ImageTexture const & k) const
	{
	  using std::size_t;
	  using std::hash;
	  using std::string;

	  // Compute individual hash values for first,
	  // second and third and combine them using XOR
	  // and bit shifting:
		// TODO: revise if necessary
	  return ((std::hash<std::shared_ptr<TextureData>>()(k.GetTextureData())
			   ^ (std::hash<std::shared_ptr<MappingLayer>>()(k.GetMappingLayer()) << 1)) >> 1)
			   ^ (std::hash<int>()((int)k.GetSamplingType()) << 1);
	}
};

