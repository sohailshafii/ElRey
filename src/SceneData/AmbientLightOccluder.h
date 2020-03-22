
#pragma once

#include "Common.h"
#include "SceneData/Light.h"

class GenericSampler;

class AmbientLightOccluder : public Light
{
public:
	AmbientLightOccluder(const Color3& radiance, float radianceScale, float minAmount, RandomSamplerType
	randomSamplerType, unsigned int numRandomSamples, unsigned int numRandomSets);
	~AmbientLightOccluder();
	
	virtual Vector3 GetDirectionFromPosition(
											 const IntersectionResult& intersectionRes) override;
	virtual Color3 GetRadiance() override;
	
	virtual bool IsAmbient() const override {
		return true;
	}
	
private:
	Color3 radiancePreScaled;
	float minAmount;
	GenericSampler* ambientSampler;
};
