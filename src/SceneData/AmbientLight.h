#pragma once

#include "SceneData/Light.h"

class AmbientLight : public Light
{
public:
	AmbientLight(const Color3& radiance, float radianceScale);
	~AmbientLight();
	
	virtual Vector3 GetDirectionFromPosition(
		const Point3& worldPosition) override;
	virtual Color3 GetRadiance() override;
	
	virtual bool IsAmbient() const override {
		return true;
	}
	
private:
	Color3 radiancePreScaled;
};
