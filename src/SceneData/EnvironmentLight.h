#pragma once

#include <memory>
#include <string>
#include "Materials/Material.h"
#include "SceneData/Light.h"
#include "Sampling/SamplerCreator.h"

// note that the primitive we store here is also traced against
// we just need to make sure that we when we intersect said
// primitive we don't trace against its own light.
class EnvironmentLight : public Light
{
public:
	EnvironmentLight(bool castsShadows,
					 RandomSamplerType randomSamplerType,
					 unsigned int numRandomSamples, unsigned int numRandomSets,
					 std::shared_ptr<Material> const& iMaterial);
	~EnvironmentLight();
	
	virtual Vector3 GetDirectionFromPositionScaled(
		const IntersectionResult& intersectionRes) const override;
	virtual Color3 GetRadiance(const IntersectionResult& intersectionRes, const Scene& scene) override;
	
	virtual bool IsAmbient() const override {
		return false;
	}

private:
	GenericSampler* sampler;
	std::shared_ptr<Material> material;
};
