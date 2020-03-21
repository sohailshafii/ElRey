
#pragma once

#include "SceneData/Light.h"

class AmbientLightOccluder : public Light
{
public:
	AmbientLightOccluder(const Color3& radiance, float radianceScale, float minAmount);
	~AmbientLightOccluder();
	
	virtual Vector3 GetDirectionFromPosition(
		const Point3& worldPosition) override;
	virtual Color3 GetRadiance() override;
	
	virtual bool IsAmbient() const override {
		return true;
	}
	
private:
	Color3 radiancePreScaled;
	float minAmount;
};
