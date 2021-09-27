#pragma once

#include <string>

// very simple container for texture data
// which can be shared among different texture instances
class TextureData {
public:
	TextureData(std::string const & filePath);
	
	~TextureData() {
		if (pixels != nullptr) {
			delete [] pixels;
		}
	}
	
	float *pixels;
	int texWidth, texHeight;
	int texChannels, rowStride;
};
