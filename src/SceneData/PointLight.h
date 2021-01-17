#pragma once

#include "SceneData/Light.h"

class PointLight : public Light
{
public:
	PointLight(bool castsShadows, const Point3 &lightPosition,
		const Color3& radiance, float radianceScale);
	~PointLight();

	virtual Vector3 GetDirectionFromPositionScaled(const ShadingInfo& shadingInfo) const override;
	virtual Color3 GetRadiance(const ShadingInfo &shadingInfo, const Scene& scene) const override;

private:
	Point3 lightPosition;
	Color3 radiancePreScaled;
};
