#include "Point3.h"
#include "Math/Vector3.h"
#include <cstring>

Point3::Point3(float x, float y, float z) {
	comp[0] = x;
	comp[1] = y;
	comp[2] = z;
}

Point3::Point3(const Point3&p2) {
	memcpy(comp, p2.comp, sizeof(float)*3);
}

Point3& Point3::operator=(const Point3& other) {
	if (this != &other) {
		memcpy(comp, other.comp, sizeof(float)*3);
	}
	return *this;
}

float Point3::operator*(const Vector3 &vector) const {
	return (comp[0]*vector[0] + comp[1]*vector[1]
		+ comp[2]*vector[2]);
}

Point3 Point3::operator+(const Vector3 &vector) const {
	return Point3(comp[0]+vector[0], comp[1]+vector[1],
		comp[2]+vector[2]);
}

Point3 Point3::operator-(const Vector3 &vector) const {
	return Point3(comp[0]-vector[0], comp[1]-vector[1],
		comp[2]-vector[2]);
}
