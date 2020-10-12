#pragma once

#include "Math/Vector3.h"
#include "Math/Point3.h"
#include <cstdint>
#include <string>

class ParamsForNormal {
public:
	ParamsForNormal() {
		
	}
	
	ParamsForNormal(Vector3 const & rayDirection, Point3 const & 			intersectionPos, float metadata, float metadata2,
					float metadata3, std::string const &primitiveName) {
		this->rayDirection = rayDirection;
		this->intersectionPos = intersectionPos;
		this->genericMetadata1 = metadata;
		this->genericMetadata2 = metadata2;
		this->genericMetadata3 = metadata3;
		this->primitiveName = primitiveName;
	}
	
	void SetRayDirection(Vector3 const & rayDirection) {
		this->rayDirection = rayDirection;
	}
	
	void SetIntersectionPosition(Point3 const & intersectionPos) {
		this->intersectionPos = intersectionPos;
	}
	
	Vector3 GetRayDirection() const {
		return rayDirection;
	}
	
	Point3 GetIntersectionPos() const {
		return intersectionPos;
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

private:
	Vector3 rayDirection;
	Point3 intersectionPos;
	float genericMetadata1;
	float genericMetadata2;
	float genericMetadata3;
	std::string primitiveName;
};
