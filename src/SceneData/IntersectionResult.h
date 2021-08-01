#pragma once

#include "Math/Ray.h"
#include "Math/Point3.h"
#include "Math/Matrix4x4.h"
#include <cstdint>
#include <string>
#include <unordered_map>

class Primitive;

class IntersectionResult {
public:
	float rayIntersectT;
	float genericMetadata1, genericMetadata2,
		genericMetadata3;
	
	// for each compound primitive, keep track of
	// which child was intersected against
	// TODO: needs to be reset?
	std::unordered_map<Primitive*, Primitive*>
		compoundPrimitiveToIntersectedPrim;
	
	void Reset() {
		compoundPrimitiveToIntersectedPrim.clear();
	}
};
