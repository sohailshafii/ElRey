#pragma once

#include "Materials/Texture.h"
#include "Math/Perlin.h"

class NoiseTexture : public Texture {
public:
	NoiseTexture() {
	}
	NoiseTexture(float sc) : scale(sc) { }

	virtual Vec3 Value(float u, float v, const Vec3& p) const {
		//return Vec3(1, 1, 1)*Perlin::Noise(scale * p);
		//return Vec3(1,1,1)*0.5*(1.0 + Perlin::Turbulence(scale*p));
		//return Vec3(1,1,1)*Perlin::Turbulence(scale*p);
		return Vec3(1,1,1)*0.5*(1.0 + sin(scale*p.x()
			+ 5*Perlin::Turbulence(scale*p)));
	}

	float scale;
};
