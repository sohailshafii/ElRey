#pragma once

#include "Math/Primitive.h"

// abstract class for all acceleration structures, or really anything
// that represents our world
class World {
public:
	World();
	~World();

	virtual void AddPrimitive(Primitive *primitive) = 0;
	virtual void AddPrimitives(Primitive *primitives,
		int numPrimitives) = 0;
	// TODO: intersection function, accessors

};
