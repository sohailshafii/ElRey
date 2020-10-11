
#pragma once

#include "Sampling/SamplerCreator.h"
#include "SceneData/Light.h"

class GenericSampler;

class AmbientLightOccluder : public Light
{
public:
	AmbientLightOccluder(const Color3& radiance,
		float radianceScale, float minAmount,
		RandomSamplerType randomSamplerType,
		unsigned int numRandomSamples, unsigned int numRandomSets);
	~AmbientLightOccluder();
	
	virtual Vector3 GetDirectionFromPositionScaled(
		const IntersectionResult& intersectionRes) const override;
	virtual Color3 GetRadiance(const IntersectionResult& intersectionRes,
							   const Scene& scene) const override;
	
	virtual bool IsAmbient() const override {
		return true;
	}
	
private:
	Color3 radiancePreScaled;
	Color3 minRadiancePreScaled;
	GenericSampler* ambientSampler;
};
