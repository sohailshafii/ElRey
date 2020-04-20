#pragma once

#include "Math/Ray.h"

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
	
	void SetIntersectionNormal(const Vector3& normalVector) {
		this->normalVector = normalVector;
	}
	
	void SetIntersectionPosition(const Point3& intersectionPos) {
		this->intersectionPosition = intersectionPos;
	}
	
	void SetIntersectionT(float rayIntersectT) {
		this->rayIntersectT = rayIntersectT;
	}

	void SetVectorToLight(const Vector3& vectorToLight) {
		this->vectorToLight = vectorToLight;
	}

	void SetSamplePointOnLight(const Point3& samplePoint) {
		samplePointOnLight = samplePoint;
	}

	const Ray GetIncomingRayInverse() const {
		return incomingRay;
	}

	const Vector3 GetIncomingDirInverse() const {
		return incomingDirInverse;
	}
	
	const Vector3 GetVectorToLight() const {
		return vectorToLight;
	}
	
	const Vector3 GetNormalVector() const {
		return normalVector;
	}
	
	const Point3 GetIntersectionPos() const {
		return intersectionPosition;
	}
	
	float GetRayIntersectT() const {
		return rayIntersectT;
	}

	Point3 GetSamplePointOnLight() const {
		return samplePointOnLight;
	}

private:
	// usually lighting deals with incoming ray facing away from surface
	Ray incomingRay;
	Vector3 vectorToLight;
	Vector3 incomingDirInverse;
	Vector3 normalVector;
	Point3 intersectionPosition;
	float rayIntersectT;

	// special stuff
	Point3 samplePointOnLight;
};
