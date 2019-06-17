#include "Point4.h"

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

int &operator[] (int index) {
	return comp[index];
}

int operator[] (int index) const {
	return comp[index];
}
