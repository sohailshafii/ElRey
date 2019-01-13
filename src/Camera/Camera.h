#pragma once

#include "../Math/Ray.h"
#include <cmath>

class Camera {
public:
	Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vup, float vfov, float aspectRatio,
		float aperture, float focusDist) {
		lensRadius = aperture*0.5;
		float theta = vfov*M_PI/180.0;
		float halfHeight = tan(theta*0.5f);
		float halfWidth = aspectRatio * halfHeight;

		origin = lookFrom;
		w = unitVector(lookFrom - lookAt); // forward
		u = unitVector(cross(vup, w)); // right vec
		v = cross(w, u); // up

		//lowerLeftCorner = Vec3(-halfWidth,-halfHeight,-1.0);
		// remember that we are looking in -w or -forward direction
		lowerLeftCorner = origin - 
			halfWidth*focusDist*u - 
			halfHeight*focusDist*v -
			focusDist*w;

		horizontal = 2.0*halfWidth*focusDist*u;
		vertical = 2.0*halfHeight*focusDist*v;
	}

	Ray GetRay(float s, float t) {
		Vec3 Rd = lensRadius*RandomInUnitDisk();
		Vec3 randomOffset = u * Rd.x() + v * Rd.y();

		return Ray(origin,
			lowerLeftCorner + s*horizontal +
			t*vertical - origin - randomOffset);
	}

	Vec3 origin, lowerLeftCorner;
	Vec3 horizontal, vertical;
	Vec3 u, v, w;
	float lensRadius;
};

