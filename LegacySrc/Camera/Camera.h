#pragma once

#include "Math/Ray.h"
#include <cmath>

class Camera {
public:
	Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vup, float vfov, float aspectRatio,
		float aperture, float focusDist, float t0, float t1) {
		time0 = t0;
		time1 = t1;

		lensRadius = aperture*0.5f;
		float theta = vfov*(float)M_PI/180.0f;
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

		horizontal = 2.0f*halfWidth*focusDist*u;
		vertical = 2.0f*halfHeight*focusDist*v;
	}

	Ray GetRay(float s, float t) {
		Vec3 Rd = lensRadius*RandomInUnitDisk();
		Vec3 randomOffset = u * Rd.x() + v * Rd.y();

		float time = time0 + getRand()*(time1 - time0);

		return Ray(origin + randomOffset,
			lowerLeftCorner + s*horizontal +
			t*vertical - origin - randomOffset,
			time);
	}

	Vec3 origin, lowerLeftCorner;
	Vec3 horizontal, vertical;
	Vec3 u, v, w;
	float lensRadius;

	// timing
	float time0, time1;
};

