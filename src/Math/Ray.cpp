#include "Ray.h"

Ray::Ray(const Point3& origin, const Vector3& direction) {
	this->origin = origin;
	this->direction = direction;
}

Ray::Ray(const Ray &other) {
	this->origin = other.origin;
	this->direction = other.direction;
}

Ray& Ray::operator=(const Ray& other) {
	if (this != &other) {
		this->origin = other.origin;
		this->direction = other.direction;
	}
	return *this;
}