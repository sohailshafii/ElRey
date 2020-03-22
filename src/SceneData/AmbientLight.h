#pragma once

#include "SceneData/Light.h"

class AmbientLight : public Light
{
public:
	AmbientLight(const Color3& radiance, float radianceScale);
	~AmbientLight();
	
	virtual Vector3 GetDirectionFromPosition(
											 const IntersectionResult& intersectionRes) override;
	virtual Color3 GetRadiance(const IntersectionResult& intersectionRes, const Scene& scene) override;
	
	virtual bool IsAmbient() const override {
		return true;
	}
	
private:
	Color3 radiancePreScaled;
};
