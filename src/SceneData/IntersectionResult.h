#pragma once

#include "Math/Ray.h"

class IntersectionResult {
public:
	IntersectionResult(Ray incomingRay, float rayIntersectT) {
		this->incomingRay = incomingRay;
		this->incomingDirInverse = -incomingRay.GetDirection();
		this->rayIntersectT = rayIntersectT;
	}

	const Ray& GetIncomingRayInverse() const {
		return incomingRay;
	}

	const Vector3& GetIncomingDirInverse() const {
		return incomingDirInverse;
	}

	float GetRayIntersectT() const {
		return rayIntersectT;
	}

private:
	// usually lighting deals with incoming ray facing away from surface
	Ray incomingRay;
	Vector3 incomingDirInverse;
	float rayIntersectT;
};
