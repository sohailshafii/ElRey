#pragma once

#include "Math/Vector3.h"
#include "Math/Point3.h"
#include <cstdint>
#include <string>

class Primitive;

class ParamsForNormal {
public:
	ParamsForNormal(Vector3 const & rayDirection,
					Point3 const & intersectionPosPrimSpace,
					float metadata, float metadata2,
					float metadata3,
					Primitive* childPrimHit) {
		this->rayDirection = rayDirection;
		this->intersectionPosPrimSpace = intersectionPosPrimSpace;
		this->genericMetadata1 = metadata;
		this->genericMetadata2 = metadata2;
		this->genericMetadata3 = metadata3;
		this->childPrimHit = childPrimHit;
	}

	Vector3 rayDirection;
	Point3 intersectionPosPrimSpace;
	float genericMetadata1;
	float genericMetadata2;
	float genericMetadata3;
	Primitive* childPrimHit;
};
