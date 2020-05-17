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
};
