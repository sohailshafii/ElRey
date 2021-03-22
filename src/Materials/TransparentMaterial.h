#pragma once

#include "Material.h"
#include "Color.h"
#include "Materials/ReflectiveMaterial.h"
#include "Materials/BRDFs/PerfectSpecularBRDF.h"
#include "Materials/BTDFs/PerfectTransmitterBTDF.h"

class GenericSampler;

class TransparentMaterial : public ReflectiveMaterial {
public:
	TransparentMaterial(float ka, float kd, float ks, float exponent, const Color3& color,
					   const Color3& ksColor, float cr, float kr, float eta, float kt);
	
	virtual void GetSecondaryVectors(ShadingInfo const & shadingInfo, Vector3 const &normal,
									 Vector3 const &incomingVecFacingAwaySurface,
									 std::vector<SecondaryVectorInfo> & secondaryVectors) const override;
	
private:
	PerfectTransmitterBTDF perfectTransmitterBTDF;
};

