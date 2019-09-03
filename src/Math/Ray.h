#pragma once

#include "Math/Point4.h"
#include "Math/Vector3.h"

class Ray {
public:
	Ray() {
		
	}
	Ray(const Point4& origin, const Vector3& direction);

	Ray(const Ray &p2);
	Ray& operator=(const Ray& other);

	const Point4& GetOrigin() const {
		return origin;
	}

	const Vector3& GetDirection() const {
		return direction;
	}

	void SetOrigin(const Point4 &newOrigin) {
		this->origin = newOrigin;
	}

	void SetDirection(const Vector3 &newDirection) {
		this->direction = newDirection;
	}

private:
	Point4 origin;
	Vector3 direction;
};
