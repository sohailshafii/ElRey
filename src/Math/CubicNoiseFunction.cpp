#include "CubicNoiseFunction.h"
#include "Math/CommonMath.h"

CubicNoiseFunction::CubicNoiseFunction(unsigned int numOctaves,
									   float lacunarity,
									   float gain)
	: LinearNoiseFunction(numOctaves, lacunarity, gain) {
}

float CubicNoiseFunction::GetValue(Point3 const & point) const {
	int ix, iy, iz;
	float fx, fy, fz;
	float xknots[4], yknots[4], zknots[4];
	
	ix = floor(point[0]);
	fx = point[0] - ix;
	
	ix = floor(point[1]);
	fy = point[1] - ix;
	
	iz = floor(point[2]);
	fz = point[2] - iz;
	
	for (int k = -1; k <= 2; k++) {
		for (int j = -1; j <= 2; j++) {
			for (int i = -1; i <= 2; i++) {
				xknots[i+1] = noiseValues[INDEX(ix + i, iy + j, iz + k)];
			}
			yknots[j+1] = FourKnotSpline(fx, xknots);
		}
		zknots[k+1] = FourKnotSpline(fy, yknots);
	}
	
	return CommonMath::Clamp(FourKnotSpline(fz, zknots), -1.0, 1.0);
}

Vector3 CubicNoiseFunction::GetVectorValue(Point3 const & point) const {
	int ix, iy, iz;
	float fx, fy, fz;
	Vector3 xknots[4], yknots[4], zknots[4];
	
	ix = floor(point[0]);
	fx = point[0] - ix;
	
	ix = floor(point[1]);
	fy = point[1] - ix;
	
	iz = floor(point[2]);
	fz = point[2] - iz;
	
	for (int k = -1; k <= 2; k++) {
		for (int j = -1; j <= 2; j++) {
			for (int i = -1; i <= 2; i++) {
				xknots[i+1] = noiseVectorValues[INDEX(ix + i, iy + j, iz + k)];
			}
			yknots[j+1] = FourKnotSplineVec(fx, xknots);
		}
		zknots[k+1] = FourKnotSplineVec(fy, yknots);
	}
	
	return FourKnotSplineVec(fz, zknots);
}
