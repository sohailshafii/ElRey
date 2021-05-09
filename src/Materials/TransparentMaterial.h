#pragma once

#include "Material.h"
#include "Color.h"
#include "Materials/ReflectiveMaterial.h"
#include "Materials/BRDFs/PerfectSpecularBRDF.h"
#include "Materials/BTDFs/PerfectTransmitterBTDF.h"

class GenericSampler;

class TransparentMaterial : public ReflectiveMaterial {
public:
	TransparentMaterial(float ka, float kd, float ks, float exponent,
						std::shared_ptr<AbstractTexture> const & color,
						std::shared_ptr<AbstractTexture> const & ksColor,
						float cr, float kr, float eta, float kt);
	
	virtual void GetSecondaryVectors(ShadingInfo const & shadingInfo,
									 std::vector<SecondaryVectorInfo> & secondaryVectors) const override;
	
private:
	PerfectTransmitterBTDF perfectTransmitterBTDF;
};

