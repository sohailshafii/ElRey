#pragma once

#include "Ray.h"

class Camera {
public:
	Camera(float aspectRatio) {
		LowerLeftCorner = Vec3(-1.0*aspectRatio,-1.0,-1.0);
		Horizontal = Vec3(2.0*aspectRatio, 0.0, 0.0);
		Vertical = Vec3(0.0, 2.0, 0.0);
		Origin = Vec3(0.0, 0.0, 0.0);
	}
	Ray GetRay(float u, float v) {
		return Ray(Origin,
			LowerLeftCorner +
			u*Horizontal + v*Vertical
			- Origin);
	}

	Vec3 Origin, LowerLeftCorner;
	Vec3 Horizontal, Vertical;
};
