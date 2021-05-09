#pragma once

#include "Material.h"
#include "Color.h"
#include "Materials/PhongMaterial.h"

class GenericSampler;

class GlossySpecularMaterial : public PhongMaterial {
public:
	GlossySpecularMaterial(float ka, float kd, float ks, float exponent,
						   std::shared_ptr<AbstractTexture> const & color,
						   std::shared_ptr<AbstractTexture> const & ksColor,
						   float cr, float kr);
	
	virtual void GetSecondaryVectors(ShadingInfo const & shadingInfo,
									 std::vector<SecondaryVectorInfo> & secondaryVectors) const override;
	
private:
	float reflectivity;
};


