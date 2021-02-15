#pragma once

#include "Math/Vector3.h"
#include "Math/Point3.h"
#include <unordered_map>

class Primitive;

class ShadingInfo {
public:
	ShadingInfo(float intGenericMetadata1,
				float intGenericMetadata2,
				float intGenericMetadata3,
				std::unordered_map<Primitive*, Primitive*>* compoundPrimitiveToIntersectedPrim,
				Vector3 const & rayDirection,
				Point3 const & intersectionPosition)
		: intGenericMetadata1(intGenericMetadata1),
		intGenericMetadata2(intGenericMetadata2),
		intGenericMetadata3(intGenericMetadata3),
		compoundPrimitiveToIntersectedPrim(compoundPrimitiveToIntersectedPrim),
		rayDirection(rayDirection),
		incomingDirInverse(-rayDirection),
		intersectionPosition(intersectionPosition) {
	}
	
	// used to obtain intersection information from a primitive
	float intGenericMetadata1, intGenericMetadata2,
		intGenericMetadata3;
	// useful for compound or grid primitives
	// this is pointer because we want to avoid copies
	std::unordered_map<Primitive*, Primitive*>*
		compoundPrimitiveToIntersectedPrim;
	
	// rayDirection = wo, incomingDirInverse = wi
	Vector3 rayDirection, incomingDirInverse;
	Point3 intersectionPosition;
	
	// All items are in world space unless specified otherwise
	// usually lighting deals with incoming ray facing away from surface
	Vector3 vectorToLight, vectorToLightScaled;

	Vector3 areaLightNormalVector;
	Vector3 normalVector;
	// special stuff
	Point3 samplePointOnLight;
};
