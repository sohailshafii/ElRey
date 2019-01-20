#pragma once

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

#include "Vec3.h"
#include "Ray.h"
#include <utility>

class AABB {
public:
	AABB() { }
	AABB(const Vec3 &a, const Vec3 &b) { _min = a; _max = b; }

	Vec3 Min() const { return _min; }
	Vec3 Max() const { return _max; }

	bool Hit(const Ray& r, float tMin, float tMax) const {
		auto rayOrigin = r.origin();
		auto rayDirection = r.direction();
		for (int a = 0; a < 3; a++) {
			float t0 = ffmin((_min[a] - rayOrigin[a])/rayDirection[a],
				(_max[a] - rayOrigin[a])/rayDirection[a]);
			float t1 = ffmax((_min[a] - rayOrigin[a])/rayDirection[a],
				(_max[a] - rayOrigin[a])/rayDirection[a]);
			// find max of mins, and min of maxs
			tMin = ffmax(t0, tMin);
			tMax = ffmin(t1, tMax);
			if (tMax <= tMin) { 
				return false;
			}
		}
		return true;
	}

	bool Hit2(const Ray& r, float tMin, float tMax) const {
		auto rayOrigin = r.origin();
		auto rayDirection = r.direction();
		for (int a = 0; a < 3; a++) {
			float invD = 1.0f/rayDirection[a];
			float t0 = (_min[a] - rayOrigin[a])*invD;
			float t1 = (_max[a] - rayOrigin[a])*invD;
			if (invD < 0.0f) {
				std::swap(t0, t1);
			}
			tMin = t0 > tMin ? t0 : tMin;
			tMax = t1 < tMax ? t1 : tMax;
			if (tMax <= tMin) {
				return false;
			}
		}
		return true;
	}

	Vec3 _min, _max;
};

AABB SurroundingBox(AABB box0, AABB box1) {
	auto box0Min = box0.Min(), box0Max = box0.Max();
	auto box1Min = box1.Min(), box1Max = box1.Max();

    Vec3 smallBox(fmin(box0Min.x(), box1Min.x()),
                fmin(box0Min.y(), box1Min.y()),
                fmin(box0Min.z(), box1Min.z()));
    Vec3 bigBox(fmax(box0Max.x(), box1Max.x()),
             fmax(box0Max.y(), box1Max.y()),
             fmax(box0Max.z(), box1Max.z()));
    return AABB(smallBox, bigBox);
}

