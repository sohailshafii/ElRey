#pragma once

#include "Math/Ray.h"
#include "Math/Matrix4x4.h"
#include <cstdint>
#include <string>

class Primitive;

class IntersectionResult {
public:
	IntersectionResult() {
	}
	
	IntersectionResult(const Ray& incomingRay, const Vector3& lightVector,
					   const Vector3& normalVector, const Point3& intersectionPosition,
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

	Ray const & GetIncomingRay() const {
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
	
	bool operator< (const IntersectionResult& other) const {
		float intersectSum1 = intersectionPosition[0] +
			intersectionPosition[1] + intersectionPosition[2];
		auto& otherPos = other.intersectionPosition;
		float intersectSum2 = otherPos[0] + otherPos[1] + otherPos[2];
		return intersectSum1 < intersectSum2;
	}
	
	// TODO: necessary for grid to intersect against same object multiple times
	// is there an alternative?
	void ResetPrimIntersectionData() {
		genericMetadata1 = 0.0f;
		genericMetadata2 = 0.0f;
		genericMetadata3 = 0.0f;
		localToWorld.MakeIdentity();
		worldToLocal.MakeIdentity();
		worldToLocalTranspose.MakeIdentity();
	}
	
	Matrix4x4 localToWorld;
	Matrix4x4 worldToLocal;
	Matrix4x4 worldToLocalTranspose;

private:
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
};
