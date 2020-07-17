#pragma once

#include "Math/Ray.h"
#include <cstdint>
#include <string>

class IntersectionResult {
public:
	IntersectionResult() {
		
	}
	
	IntersectionResult(const Ray& incomingRay, const Vector3& lightVector, const Vector3& normalVector, const Point3& intersectionPosition,
					   float rayIntersectT) {
		this->incomingRay = incomingRay;
		this->vectorToLight = lightVector;
		this->normalVector = normalVector;
		this->intersectionPosition = intersectionPosition;
		this->incomingDirInverse = -incomingRay.GetDirection();
		this->rayIntersectT = rayIntersectT;
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
	
	void SetGenericMetadata(float metadata, float metadata2,
							float metadata3) {
		genericMetadata1 = metadata;
		genericMetadata2 = metadata2;
		genericMetadata3 = metadata3;
	}
	
	void SetPrimitiveName(std::string const & primitiveName) {
		this->primitiveName = primitiveName;
	}

	Ray GetIncomingRayInverse() const {
		return incomingRay;
	}

	Vector3 GetIncomingDirInverse() const {
		return incomingDirInverse;
	}
	
	Vector3 GetVectorToLight() const {
		return vectorToLight;
	}

	Vector3 GetVectorToLightScaled() const {
		return vectorToLightScaled;
	}
	
	Vector3 GetNormalVector() const {
		return normalVector;
	}

	Vector3 GetAreaLightNormal() const {
		return areaLightNormalVector;
	}
	
	Point3 GetIntersectionPos() const {
		return intersectionPosition;
	}
	
	float GetRayIntersectT() const {
		return rayIntersectT;
	}

	Point3 GetSamplePointOnLight() const {
		return samplePointOnLight;
	}
	
	float GetGenericMetadata1() const {
		return genericMetadata1;
	}
	
	float GetGenericMetadata2() const {
		return genericMetadata2;
	}
	
	float GetGenericMetadata3() const {
		return genericMetadata3;
	}
	
	std::string GetPrimitiveName() const {
		return primitiveName;
	}
	
	bool operator< (const IntersectionResult& other) const {
		float intersectSum1 = intersectionPosition[0] +
			intersectionPosition[1] + intersectionPosition[2];
		auto& otherPos = other.intersectionPosition;
		float intersectSum2 = otherPos[0] + otherPos[1] + otherPos[2];
		return intersectSum1 < intersectSum2;
	}

private:
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
	// TODO: enforce uniqueness in scene when it comes to names
	// useful for compound objects
	std::string primitiveName;
};
