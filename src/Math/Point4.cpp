#include "Point4.h"
#include "Math/Vector3.h"
#include <cstring>

Point4::Point4(float x, float y, float z) {
	comp[0] = x;
	comp[1] = y;
	comp[2] = z;
	comp[3] = 1.0;
}

Point4::Point4(float x, float y, float z, float w) {
	comp[0] = x;
	comp[1] = y;
	comp[2] = z;
	comp[3] = w;
}

Point4::Point4(const Point4 &p2) {
	memcpy(comp, p2.comp, sizeof(float)*4);
}

Point4& Point4::operator=(const Point4& other) {
	if (this != &other) {
		memcpy(comp, other.comp, sizeof(float)*4);
	}
	return *this;
}

float Point4::operator*(const Vector3 &vector) const {
	return (comp[0]*vector[0] + comp[1]*vector[1]
		+ comp[2]*vector[2]);
}


Point4 Point4::operator+(const Vector3 &vector) const {
	return Point4(comp[0]+vector[0], comp[1]+vector[1],
		comp[2]+vector[2], comp[3]);
}

Point4 Point4::operator-(const Vector3 &vector) const {
	return Point4(comp[0]-vector[0], comp[1]-vector[1],
		comp[2]-vector[2], comp[3]);
}
