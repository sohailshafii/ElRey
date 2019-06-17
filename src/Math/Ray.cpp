#include "Ray.h"

Ray::Ray(const Point4& origin, const Vector3& direction) {
	this->origin = origin;
	this->direction = direction;
}

Ray::Ray(const Ray &p2) {
	this->origin = p2.origin;
	this->direction = p2.direction;
}

Ray& Ray::operator=(const Ray& other) {
	if (this != &other) {
		this->origin = other.origin;
		this->direction = other.direction;
	}
	return *this;
}