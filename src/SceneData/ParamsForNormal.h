#pragma once

#include "Math/Vector3.h"
#include "Math/Point3.h"
#include <cstdint>
#include <string>

class Primitive;

class ParamsForNormal {
public:
	ParamsForNormal(Vector3 const & rayDirection,
					Point3 const & intersectionPos,
					float metadata, float metadata2,
					float metadata3) {
		this->rayDirection = rayDirection;
		this->intersectionPos = intersectionPos;
		this->genericMetadata1 = metadata;
		this->genericMetadata2 = metadata2;
		this->genericMetadata3 = metadata3;
	}

	Vector3 rayDirection;
	Point3 intersectionPos;
	float genericMetadata1;
	float genericMetadata2;
	float genericMetadata3;
};
