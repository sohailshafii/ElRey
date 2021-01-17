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

	// All items are in world space unless specified otherwise
	// usually lighting deals with incoming ray facing away from surface
	Vector3 vectorToLight, vectorToLightScaled;

	Vector3 areaLightNormalVector;
	Vector3 incomingDirInverse;
	Vector3 normalVector;
	Point3 intersectionPosition;
	// special stuff
	Point3 samplePointOnLight;
	
	float rayIntersectT;
	float genericMetadata1, genericMetadata2,
		genericMetadata3;
	// useful for instance primitives
	Primitive* childPrimitiveHit;
};
