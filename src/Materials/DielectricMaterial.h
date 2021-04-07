#pragma once

#include "Material.h"
#include "Color.h"
#include "Materials/ReflectiveMaterial.h"
#include "Materials/BRDFs/PerfectSpecularBRDF.h"
#include "Materials/BTDFs/PerfectTransmitterBTDF.h"

class GenericSampler;

class DielectricMaterial : public PhongMaterial {
public:
	DielectricMaterial(float ka, float kd, float ks, float exponent,
					   const Color3& color, const Color3& ksColor,
					   float etaIn, float etaOut,
					   Color3 const & cfIn, Color3 const & cfOut);
	
	virtual void GetSecondaryVectors(ShadingInfo const & shadingInfo,
									 std::vector<SecondaryVectorInfo> & secondaryVectors) const override;
	
private:
	PerfectTransmitterBTDF perfectTransmitterBTDF;
	float etaOut;
	Color3 cfIn;
	Color3 cfOut;
};


