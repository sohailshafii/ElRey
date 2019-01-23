#pragma once

class Perlin {
public:
	static float Noise(const Vec3 p) {
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());

		int i = int(4*p.x()) & 255;
		int j = int(4*p.y()) & 255;
		int k = int(4*p.z()) & 255;
		return ranfloat[permX[i] ^ permY[j] ^ permZ[k]];
	}
	static float *ranfloat;
	static int *permX, *permY, *permZ;
};

static float* perlinGenerate() {
	float * p = new float[256];
	for (int i = 0; i < 256; ++i) {
		p[i] = drand48();
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

float *Perlin::ranfloat = perlinGenerate();
int *Perlin::permX = perlinGeneratePerm();
int *Perlin::permY = perlinGeneratePerm();
int *Perlin::permZ = perlinGeneratePerm();




