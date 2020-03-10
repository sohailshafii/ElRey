#pragma once

#include "SceneData/Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(const Vector3& direction, const Color3& radiance,
					 float radianceScale);
	~DirectionalLight();
	
	virtual Vector3 GetDirectionFromPosition(const Point3& worldPosition) override;
	virtual Color3 GetRadiance() override;
	
	virtual bool CastsShadows() override {
		return true;
	}
	
	virtual bool IsLightDistanceInfinite() const override {
		return true;
	}
	
private:
	Vector3 lightDirection;
	Color3 radiancePreScaled;
};

