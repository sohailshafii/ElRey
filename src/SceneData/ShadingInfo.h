#pragma once

#include "Math/Vector3.h"
#include "Math/Point3.h"

class Primitive;

class ShadingInfo {
public:
	ShadingInfo(float intGenericMetadata1,
				float intGenericMetadata2,
				float intGenericMetadata3,
				Primitive* childPrimitiveHit,
				Vector3 const & rayDirection,
				Point3 const & intersectionPosition)
		: intGenericMetadata1(intGenericMetadata1),
		intGenericMetadata2(intGenericMetadata2),
		intGenericMetadata3(intGenericMetadata3),
		childPrimitiveHit(childPrimitiveHit),
		rayDirection(rayDirection),
		incomingDirInverse(-rayDirection),
		intersectionPosition(intersectionPosition) {
	}
	
	float intGenericMetadata1, intGenericMetadata2,
		intGenericMetadata3;
	// useful for instance primitives
	Primitive* childPrimitiveHit;
	
	// All items are in world space unless specified otherwise
	// usually lighting deals with incoming ray facing away from surface
	Vector3 vectorToLight, vectorToLightScaled;

	Vector3 areaLightNormalVector;
	Vector3 rayDirection, incomingDirInverse;
	Vector3 normalVector;
	Point3 intersectionPosition;
	// special stuff
	Point3 samplePointOnLight;
};
