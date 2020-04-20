#pragma once

#include "SceneData/Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(bool castsShadows, const Vector3& direction, const Color3& radiance,
					 float radianceScale);
	~DirectionalLight();
	
	virtual Vector3 GetDirectionFromPosition(
	const IntersectionResult& intersectionRes) const override;
	virtual Color3 GetRadiance(const IntersectionResult& intersectionRes, const Scene& scene) override;
	
	virtual bool IsLightDistanceInfinite() const override {
		return true;
	}
	
private:
	Vector3 lightDirection;
	Color3 radiancePreScaled;
};

