#pragma once

#include "Materials/Texturing/SphericalMapping.h"
#include "Math/CommonMath.h"
#include <cmath>

class SphericalMapping : public MappingLayer {
public:
	SphericalMapping(float radiusLocal, Vector3 const &origin) : radiusLocalInv(1.0f/radiusLocal), origin(origin) {}
	
	virtual void ComputeTextureCoordinates(ShadingInfo const & shadingInfo,
										   int width, int height,
										   int & row, int & column) override {
		Point3 localHitPoint = shadingInfo.intersectionPositionLocal;
		localHitPoint = invTransformMatrix * localHitPoint;
		
		// handle cases where local hit point might be larger than radius
		float theta = acos((origin[0]-localHitPoint[1])*radiusLocalInv);
		float phi = atan2((origin[0]-localHitPoint[0])*radiusLocalInv,
						  (origin[2]-localHitPoint[2])*radiusLocalInv);
		// atan2 returns [-pi, pi], so move to [0, 2*pi]
		if (phi < 0.0f) {
			phi += TWO_PI;
		}
		
		// next, map theta and phi to [0, 1] x [0, 1]
		float u = phi * INV_TWO_PI;
		float v = 1.0f - theta * INV_PI;
		
		// finally, map u and v to the texel coordinates
		column = (int)((width - 1) * u);
		// remember that row is flipped in image space
		row = (height - 1) - (int)((height - 1) * v);
		CommonMath::Clamp(column, 0, width - 1);
		CommonMath::Clamp(row, 0, height - 1);
	}
	
private:
	Vector3 origin;
	float radiusLocalInv;
};
