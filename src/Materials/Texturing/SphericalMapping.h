#pragma once

#include "Materials/Texturing/SphericalMapping.h"
#include "Math/CommonMath.h"
#include <cmath>

class SphericalMapping : public MappingLayer {
public:
	SphericalMapping(float radiusLocal, Vector3 const &origin,
					 WrapType wrapType, bool useWorldCoordsForTex) :
		MappingLayer(wrapType, useWorldCoordsForTex), radiusLocalInv(1.0f/radiusLocal), origin(origin) {}
	
	virtual void ComputeTextureCoordinates(ShadingInfo const & shadingInfo,
										   int width, int height,
										   float & row, float & column) override {
		Point3 hitPnt = useWorldCoordsForTex ?
			shadingInfo.intersectionPosition : shadingInfo.intersectionPositionLocal;
		hitPnt = invTransformMatrix * hitPnt;
		
		// handle cases where local hit point might be larger than radius
		float theta = acos((origin[0]-hitPnt[1])*radiusLocalInv);
		float phi = atan2((origin[0]-hitPnt[0])*radiusLocalInv,
						  (origin[2]-hitPnt[2])*radiusLocalInv);
		// atan2 returns [-pi, pi], so move to [0, 2*pi]
		if (phi < 0.0f) {
			phi += TWO_PI;
		}
		
		// next, map theta and phi to [0, 1] x [0, 1]
		float u = phi * INV_TWO_PI;
		float v = 1.0f - theta * INV_PI;
		
		// finally, map u and v to the texel coordinates
		column = (float)(width - 1) * u;
		// remember that row is flipped in image space
		row = (float)(height - 1) - (float)(height - 1) * v;
	}
	
private:
	Vector3 origin;
	float radiusLocalInv;
};
