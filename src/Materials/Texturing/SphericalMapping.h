#pragma once

#include "Materials/Texturing/SphericalMapping.h"
#include "Math/CommonMath.h"
#include <cmath>

class SphericalMapping : public MappingLayer {
public:
	SphericalMapping(float radiusLocal) : radiusLocal(radiusLocal) {}
	
	virtual void ComputeTextureCoordinates(ShadingInfo const & shadingInfo,
										   int width, int height,
										   int & row, int & column) override {
		auto const & localHitPoint = shadingInfo.intersectionPositionLocal;
		float theta = acos(localHitPoint[1]/radiusLocal);
		float phi = atan2(localHitPoint[0]/radiusLocal, localHitPoint[2]/radiusLocal);
		// atan2 returns [-pi, pi], so move to [0, 2*pi]
		if (phi < 0.0f) {
			phi += TWO_PI;
		}
		
		// next, map theta and phi to [0, 1] x [0, 1]
		float u = phi * INV_TWO_PI;
		float v = 1.0f - theta * INV_PI;
		
		// finally, map u and v to the texel coordinates
		column = (int)((width - 1) * u);
		row = (int)((height - 1) * v);
	}
	
private:
	float radiusLocal;
};
