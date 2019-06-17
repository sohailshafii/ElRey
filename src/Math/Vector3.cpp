#include "Vector3.h"
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

float &Vector3::operator[] (int index) {
	return comp[index];	
}

float Vector3::operator[] (int index) const {
	return comp[index];	
}
