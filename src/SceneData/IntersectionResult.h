#pragma once

#include "Math/Ray.h"

class IntersectionResult {
public:
	IntersectionResult(Ray incomingRay, Vector3 lightRay, float rayIntersectT) {
		this->incomingRay = incomingRay;
		this->vectorToLight = lightRay;
		this->incomingDirInverse = -incomingRay.GetDirection();
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
	float rayIntersectT;
};
