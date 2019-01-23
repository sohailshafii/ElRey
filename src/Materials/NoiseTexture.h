#pragma once

#include "Materials/Texture.h"
#include "Math/Perlin.h"

class NoiseTexture : public Texture {
public:
	NoiseTexture() {
	}

	virtual Vec3 Value(float u, float v, const Vec3& p) const {
		return Vec3(1, 1, 1)*Perlin::Noise(p);
	}

	
};
