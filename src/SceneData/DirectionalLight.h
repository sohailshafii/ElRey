#pragma once

#include "SceneData/Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(bool castsShadows, const Vector3& direction,
		const Color3& radiance, float radianceScale);
	~DirectionalLight();
	
	virtual Vector3 GetDirectionFromPositionScaled(const ShadingInfo& shadingInfo) const override;
	virtual Color3 GetRadiance(const ShadingInfo& shadingInfo, const Scene& scene) const override;
	
	virtual bool IsLightDistanceInfinite() const override {
		return true;
	}
	
private:
	Vector3 lightDirection;
	Color3 radiancePreScaled;
};

