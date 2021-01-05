#pragma once

#include <string>
#include <vector>
#include "ThirdParty/nlohmann/json.hpp"

class Primitive;
class Ray;
class IntersectionResult;

class BaseAccelerator {
public:
	enum AcceleratorType { SimpleWorld = 0, Grid };

	BaseAccelerator() { }
	BaseAccelerator(Primitive **primitives, unsigned int numPrimitives);
	
	virtual ~BaseAccelerator();
	
	// call to set accelerator after modifying primitives
	virtual void SetUpAccelerator(nlohmann::json const & jsonObj) = 0;

	void AddPrimitive(Primitive *newPrimitive);
	void AddPrimitives(Primitive **newPrimitives,
					   unsigned int numNewPrimitives);
	void AddPrimitives(std::vector<Primitive*> newPrimitives);

	void RemovePrimitive(Primitive* primitiveToRemove);
	Primitive* GetPrimitive(unsigned int index);
	Primitive* FindPrimitiveByName(const std::string& name);

	unsigned int GetNumPrimitives() const {
		return primitives.size();
	}
	
	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) = 0;
	
	virtual bool ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
												float tMax,
												const Primitive* primitiveToExclude) = 0;

protected:
	std::vector<Primitive*> primitives;
};
