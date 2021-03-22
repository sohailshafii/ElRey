#pragma once

#include "Material.h"
#include "Color.h"
#include "Materials/PhongMaterial.h"

class GenericSampler;

class GlossySpecularMaterial : public PhongMaterial {
public:
	GlossySpecularMaterial(float ka, float kd, float ks, float exponent,
						   const Color3& color, const Color3& ksColor, float cr, float kr);
	
	virtual void GetSecondaryVectors(ShadingInfo const & shadingInfo, Vector3 const &normal,
									 Vector3 const &incomingVecFacingAwaySurface,
									 std::vector<SecondaryVectorInfo> & secondaryVectors) const override;
	
private:
	float reflectivity;
};


