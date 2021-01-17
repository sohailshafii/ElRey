#pragma once

#include <vector>

class Primitive;
class Ray;
class IntersectionResult;

class SimpleWorld {
public:
	SimpleWorld();
	SimpleWorld(std::shared_ptr<Primitive> *primitives,
				unsigned int numPrimitives);
	
	~SimpleWorld();
	
	void AddPrimitive(std::shared_ptr<Primitive> newPrimitive);
	void AddPrimitives(std::shared_ptr<Primitive> *newPrimitives,
					   unsigned int numNewPrimitives);
	void AddPrimitives(std::vector<std::shared_ptr<Primitive>> newPrimitives);

	void RemovePrimitive(std::shared_ptr<Primitive> primitiveToRemove);
	void RemovePrimitiveByName(std::string const & name);
	std::shared_ptr<Primitive> GetPrimitive(unsigned int index);
	std::shared_ptr<Primitive> FindPrimitiveByName(const std::string& name);

	unsigned int GetNumPrimitives() const {
		return primitives.size();
	}
	
	Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
						IntersectionResult &intersectionResult);
	
	Primitive* ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
											  float tMax);
	
private:
	std::vector<std::shared_ptr<Primitive>> primitives;
};