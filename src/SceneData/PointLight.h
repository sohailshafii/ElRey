#pragma once

#include "SceneData/Light.h"

class PointLight : public Light
{
public:
	PointLight(const Point3 &lightPosition,
		const Color3& radiance, float radianceScale);
	~PointLight();

	virtual Vector3 GetDirectionFromPosition(
		const Point3& worldPosition) override;
	virtual Color3 GetRadiance() override;
	
	virtual bool CastsShadows() override {
		return true;
	}

private:
	Point3 lightPosition;
	Color3 radiancePreScaled;
};
