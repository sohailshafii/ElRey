#pragma once

#include "Math/Point3.h"
#include "Math/Vector3.h"

class Ray {
public:
	Ray() {
	}
	Ray(const Point3& origin, const Vector3& direction);

	Ray(const Ray &p2);
	Ray& operator=(const Ray& other);

	const Point3& GetOrigin() const {
		return origin;
	}

	const Vector3& GetDirection() const {
		return direction;
	}

	void SetOrigin(const Point3 &newOrigin) {
		this->origin = newOrigin;
	}

	void SetDirection(const Vector3 &newDirection) {
		this->direction = newDirection;
	}
	
	Point3 GetPositionAtParam(float param) const {
		return origin + direction*param;
	}

private:
	Point3 origin;
	Vector3 direction;
};
