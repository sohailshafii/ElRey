#pragma once

#include "Math/Vec3.h"
#include "Materials/Texture.h"

class ConstantTexture : public Texture {
public:
	ConstantTexture() { }
	ConstantTexture(Vec3 color) : Color(color) {
	}

	virtual Vec3 Value(float u, float v, const Vec3 &p) const {
		return Color;
	}


	Vec3 Color;
};
