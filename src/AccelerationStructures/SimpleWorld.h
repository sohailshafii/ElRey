#pragma once

#include "AccelerationStructures/BaseAccelerator.h"

class SimpleWorld : public BaseAccelerator {
public:
	SimpleWorld();
	SimpleWorld(Primitive **primitives,
				unsigned int numPrimitives);
	
	virtual void SetUpAccelerator(nlohmann::json const & jsonObj) override;
	
	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
								 IntersectionResult &intersectionResult) override;
	
	virtual Primitive* ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
													  float tMax) override;
	
protected:
	
private:
	
};
