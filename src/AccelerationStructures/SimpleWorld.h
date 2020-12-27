#pragma once

#include "AccelerationStructures/BaseAccelerator.h"

class SimpleWorld : public BaseAccelerator {
public:
	SimpleWorld();
	SimpleWorld(Primitive **primitives,
				unsigned int numPrimitives);
	
	virtual void SetUpAccelerator() override;
	
	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
								 IntersectionResult &intersectionResult) override;
	
	virtual bool ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
												float tMax,
												const Primitive* primitiveToExclude) override;
	
protected:
	
private:
	
};
