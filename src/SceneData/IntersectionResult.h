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
	
	void SetIncomingDirection(const Ray& incomingRay) {
		this->incomingRay = incomingRay;
		this->incomingDirInverse = -incomingRay.GetDirection();
	}
	
	void SetLightRay(const Vector3& lightVector) {
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
	
	void SetVectorToLight(const Vector3 &vectorToLight) {
		this->vectorToLight = vectorToLight;
	}

	float GetRayIntersectT() const {
		return rayIntersectT;
	}

private:
	// usually lighting deals with incoming ray facing away from surface
	Ray incomingRay;
	Vector3 vectorToLight;
	Vector3 incomingDirInverse;
	Vector3 normalVector;
	Point3 intersectionPosition;
	float rayIntersectT;
};
