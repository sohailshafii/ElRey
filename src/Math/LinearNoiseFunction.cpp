#include "LinearNoiseFunction.h"
#include "Math/CommonMath.h"
#include <cstdlib>

LinearNoiseFunction::LinearNoiseFunction(unsigned int numOctaves,
										 float lacunarity,
										 float gain,
										 float startingFrequency)
	: NoiseFunction(numOctaves, lacunarity, gain, startingFrequency) {
}

float LinearNoiseFunction::GetValueInterpolated(Point3 const & point) const {
	int 	ix, iy, iz;
	float 	fx, fy, fz;
	float 	d[2][2][2];
	float 	x0, x1, x2, x3, y0, y1, z0;
	
	ix = floor(point[0]);
	fx = point[0] - ix;
	
	iy = floor(point[1]);
	fy = point[1] - iy;
	
	iz = floor(point[2]);
	fz = point[2] - iz;
	
	for (int k = 0; k <= 1; k++) {
		for (int j = 0; j <= 1; j++) {
			for (int i = 0; i <= 1; i++) {
				int indexVal = INDEX(ix + i, iy + j, iz + k);
				d[k][j][i] = noiseValues[indexVal];
			}
		}
	}
		
	x0 = Lerp(fx, d[0][0][0], d[0][0][1]);
	x1 = Lerp(fx, d[0][1][0], d[0][1][1]);
	x2 = Lerp(fx, d[1][0][0], d[1][0][1]);
	x3 = Lerp(fx, d[1][1][0], d[1][1][1]);
	y0 = Lerp(fy, x0, x1);
	y1 = Lerp(fy, x2, x3);
	z0 = Lerp(fz, y0, y1);
	
	return z0;
}

Vector3 LinearNoiseFunction::GetVectorValueInterpolated(Point3 const & point) const {
	unsigned int 	ix, iy, iz;
	float 	fx, fy, fz;
	Vector3 	d[2][2][2];
	Vector3 	x0, x1, x2, x3, y0, y1, z0;
	
	ix = floor(point[0]);
	fx = point[0] - ix;
	
	iy = floor(point[1]);
	fy = point[1] - iy;
	
	iz = floor(point[2]);
	fz = point[2] - iz;
	
	for (int k = 0; k <= 1; k++) {
		for (int j = 0; j <= 1; j++) {
			for (int i = 0; i <= 1; i++) {
				d[k][j][i] =
				noiseVectorValues[INDEX(ix + i, iy + j, iz + k)];
			}
		}
	}
		
	x0 = LerpVector(fx, d[0][0][0], d[0][0][1]);
	x1 = LerpVector(fx, d[0][1][0], d[0][1][1]);
	x2 = LerpVector(fx, d[1][0][0], d[1][0][1]);
	x3 = LerpVector(fx, d[1][1][0], d[1][1][1]);
	y0 = LerpVector(fy, x0, x1);
	y1 = LerpVector(fy, x2, x3);
	z0 = LerpVector(fz, y0, y1);
	
	return z0;
}



