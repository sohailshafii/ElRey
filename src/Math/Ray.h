#pragma once

#include "Math/Point4.h"
#include "Math/Vector3.h"

class Ray {
public:
	Ray(Point4 origin, Vector3 direction);

	Ray(const Ray &p2);
	Ray& operator=(const Ray& other);

private:
	Point4 origin;
	Vector3 direction;
};
