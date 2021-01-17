#pragma once

#include <vector>

class Primitive;
class Ray;
class IntersectionResult;

class SimpleWorld {
public:
	SimpleWorld();
	SimpleWorld(Primitive **primitives,
				unsigned int numPrimitives);
	
	~SimpleWorld();
	
	void AddPrimitive(Primitive *newPrimitive);
	void AddPrimitives(Primitive **newPrimitives,
					   unsigned int numNewPrimitives);
	void AddPrimitives(std::vector<Primitive*> newPrimitives);

	void RemovePrimitive(Primitive* primitiveToRemove);
	void RemovePrimitiveByName(std::string const & name);
	Primitive* GetPrimitive(unsigned int index);
	Primitive* FindPrimitiveByName(const std::string& name);

	unsigned int GetNumPrimitives() const {
		return primitives.size();
	}
	
	Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
						IntersectionResult &intersectionResult);
	
	Primitive* ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
											  float tMax);
	
private:
	std::vector<Primitive*> primitives;
};
