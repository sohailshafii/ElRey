#pragma once

#include "Math/Primitive.h"

// simple base class for all acceleration structures, or really anything
// that represents our world.
class World {
public:
	World();
	World(Primitive **primitives, unsigned int numPrimitives);
	virtual ~World();

	virtual void AddPrimitive(Primitive *newPrimitive);
	virtual void AddPrimitives(Primitive **newPrimitives,
		unsigned int numNewPrimitives);
	virtual bool Intersect(const Ray &ray, Color &newColor,
		float tMin, float& tMax);

	Primitive* GetPrimitive(unsigned int index) {
		return primitives[index];	
	}

	unsigned int GetNumPrimitives() const {
		return numPrimitives;
	}

private:
	void cleanUpPrimitives(Primitive **primitivesToClean, unsigned int
		numToClean);

protected:
	// each item is a pointer to a primitive type,
	// whatever that might be
	Primitive** primitives;
	unsigned int numPrimitives;
};
