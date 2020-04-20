#pragma once

#include "SceneData/Light.h"

class PointLight : public Light
{
public:
	PointLight(bool castsShadows, const Point3 &lightPosition,
		const Color3& radiance, float radianceScale);
	~PointLight();

	virtual Vector3 GetDirectionFromPosition(
		const IntersectionResult& intersectionRes) const override;
	virtual Color3 GetRadiance(const IntersectionResult& intersectionRes, const Scene& scene) override;

private:
	Point3 lightPosition;
	Color3 radiancePreScaled;
};
