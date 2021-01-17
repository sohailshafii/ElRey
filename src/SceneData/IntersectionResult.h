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
	
	IntersectionResult(const Ray& incomingRay, const Vector3& lightVector,
					   const Vector3& normalVector, const Point3& intersectionPosition,
					   float rayIntersectT){
		this->incomingRay = incomingRay;
		this->vectorToLight = lightVector;
		this->normalVector = normalVector;
		this->intersectionPosition = intersectionPosition;
		this->incomingDirInverse = -incomingRay.GetDirection();
		this->rayIntersectT = rayIntersectT;
		childPrimitiveHit = nullptr;
	}
	
	void SetIncomingRay(const Ray& incomingRay) {
		this->incomingRay = incomingRay;
		this->incomingDirInverse = -incomingRay.GetDirection();
	}
	
	void SetLightVector(const Vector3& lightVector) {
		this->vectorToLight = lightVector;
	}

	void SetLightVectorScaled(const Vector3 lightVectorScaled) {
		this->vectorToLightScaled = lightVectorScaled;
	}
	
	void SetIntersectionNormal(const Vector3& normalVector) {
		this->normalVector = normalVector;
	}

	void SetAreaLightNormal(const Vector3& areaLightNormal) {
		this->areaLightNormalVector = areaLightNormal;
	}
	
	void SetIntersectionPosition(const Point3& intersectionPos) {
		this->intersectionPosition = intersectionPos;
	}
	
	void SetIntersectionT(float rayIntersectT) {
		this->rayIntersectT = rayIntersectT;
	}

	void SetSamplePointOnLight(const Point3& samplePoint) {
		samplePointOnLight = samplePoint;
	}
	
	// is there an alternative? do we need this for simple world class too?
	// seems to be necessary for compound objects as well...if the same intersection
	// results are used over and over then the previous tests will interfere with future ones
	// TODO: clean-up implementation
	void ResetPrimIntersectionData() {
		genericMetadata1 = 0.0f;
		genericMetadata2 = 0.0f;
		genericMetadata3 = 0.0f;
		childPrimitiveHit = nullptr;
	}

	// All items are in world space unless specified otherwise
	// usually lighting deals with incoming ray facing away from surface
	Ray incomingRay;
	Vector3 vectorToLight, vectorToLightScaled;

	Vector3 areaLightNormalVector;
	Vector3 incomingDirInverse;
	Vector3 normalVector;
	Point3 intersectionPosition;
	float rayIntersectT;

	// special stuff
	Point3 samplePointOnLight;
	float genericMetadata1, genericMetadata2,
		genericMetadata3;
	
	// useful for instance primitives
	Primitive* childPrimitiveHit;
};
