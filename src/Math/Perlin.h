#pragma once

#include <cmath>
#include "Math/Common.h"

float PerlinInterp(Vec3 c[2][2][2],
	float u, float v, float w);

class Perlin {
public:
	static float Noise(const Vec3 p) {
		int i = floor(p.x());
		int j = floor(p.y());
		int k = floor(p.z());

		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());

		Vec3 c[2][2][2];
		for (int di = 0; di < 2; di++) {
			for (int dj = 0; dj < 2; dj++) {
				for (int dk = 0; dk < 2; dk++) {
					c[di][dj][dk] =
						ranVec[permX[(i+di) & 255] ^ permY[(j+dj) & 255] ^ 
							permZ[(k+dk) & 255]];
				}
			}
		}
		return PerlinInterp(c, u, v, w);
	}

	// sum multiple frequencies
	static float Turbulence(const Vec3& p, int depth=7) {
		float accum = 0.0;
		Vec3 tempP = p;
		float weight = 1.0;
		for (int i = 0; i < depth; i++) {
			accum += weight*Noise(tempP);
			weight *= 0.5;
			tempP *= 2.0;
		}
		return fabs(accum);
	}

	static Vec3 *ranVec;
	static int *permX, *permY, *permZ;
};

inline float PerlinInterp(Vec3 c[2][2][2],
	float u, float v, float w) {
	float uu = u*u*(3.0-2.0*u);
	float vv = v*v*(3.0-2.0*v);
	float ww = w*w*(3.0-2.0*w);
	float accum = 0.0;

	// TODO: do this without a loop
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				Vec3 weightV(u - i, v - j, w - k);
				accum += (i*uu + (1-i)*(1-uu))*
					(j*vv + (1-j)*(1-vv))*
					(k*ww + (1-k)*(1-ww))*
					dot(c[i][j][k], weightV);
			}
		}
	}

	return accum;
}

static Vec3* perlinGenerate() {
	Vec3 * p = new Vec3[256];
	for (int i = 0; i < 256; ++i) {
		p[i] = unitVector(Vec3(-1.0 + 2.0*drand48(),
			-1.0 + 2.0*drand48(),
			-1.0 + 2.0*drand48()));
	}
	return p;
}

void permute(int *p, int n) {
	for (int i = n-1; i < 0; i--) {
		int target = int(drand48()*(i + 1));
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}
	return;
}

static int* perlinGeneratePerm() {
	int *p = new int[256];
	for (int i = 0; i < 256; i++) {
		p[i] = i;
	}
	permute(p, 256);
	return p;
}

Vec3 *Perlin::ranVec = perlinGenerate();
int *Perlin::permX = perlinGeneratePerm();
int *Perlin::permY = perlinGeneratePerm();
int *Perlin::permZ = perlinGeneratePerm();




