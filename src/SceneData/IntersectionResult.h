#pragma once

#include "Math/Ray.h"
#include "Math/Matrix4x4.h"
#include <cstdint>
#include <string>

class Primitive;

class IntersectionResult {
public:
	IntersectionResult()
	{
		childPrimitiveHit = nullptr;
	}
	
	float rayIntersectT;
	float genericMetadata1, genericMetadata2,
		genericMetadata3;
	// useful for instance primitives
	Primitive* childPrimitiveHit;
};
