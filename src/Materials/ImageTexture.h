#pragma once

#include "Texture.h"

class ImageTexture : public Texture {
public:
	ImageTexture() { data = nullptr; }
	ImageTexture(unsigned char *pixels, int A, int B) : data(pixels),
		nx(A), ny(B) { }

	~ImageTexture() {
		if (data != nullptr) {
			delete [] data;
		}
	}

	virtual Vec3 Value(float u, float v, const Vec3& p) const;

	unsigned char *data;
	static int counter;
	int nx, ny;
};

int ImageTexture::counter = 0;

Vec3 ImageTexture::Value(float u, float v, const Vec3& p) const {
	int i = (int)((u)*(float)nx);
	int j = (int)((1.0f-v)*(float)ny-0.001f);

	if (i < 0) i = 0;
	if (j < 0) j = 0;
	if (i > nx-1) i = nx-1;
	if (j > ny-1) j = ny-1;

	float r = int(data[3*i + 3*nx*j])/255.0f;
	float g = int(data[3*i + 3*nx*j+1])/255.0f;
	float b = int(data[3*i + 3*nx*j+2])/255.0f;
	return Vec3(r, g, b);
}

