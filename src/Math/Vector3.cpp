#include "Vector3.h"
#include "Math/Point3.h"
#include "Math/Point4.h"
#include <cstring>

Vector3::Vector3(float x, float y, float z) {
	comp[0] = x;
	comp[1] = y;
	comp[2] = z;
}

Vector3::Vector3(const Vector3 &p2) {
	memcpy(comp, p2.comp, sizeof(float)*3);
}

Vector3& Vector3::operator=(const Vector3& other) {
	if (this != &other) {
		memcpy(comp, other.comp, sizeof(float)*3);
	}
	return *this;
}

float Vector3::operator*(const Point3& point) const {
	return (comp[0] * point[0] + comp[1] * point[1]
		+ comp[2] * point[2]);
}

Point3 Vector3::operator+(const Point3& point) const {
	return Point3(comp[0] + point[0], comp[1] + point[1],
		comp[2] + point[2]);
}

float Vector3::operator*(const Point4 &point) const {
	return (comp[0]*point[0] + comp[1]*point[1]
		+ comp[2]*point[2]);
}

Point4 Vector3::operator+(const Point4 &point) const {
	return Point4(comp[0]+point[0], comp[1]+point[1],
		comp[2]+point[2], point[3]);
}
