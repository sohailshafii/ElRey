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

float Point4::GetDistanceSquared(
	const Point4& otherPoint) const {
	float diffX = otherPoint[0] - comp[0];
	float diffY = otherPoint[1] - comp[1];
	float diffZ = otherPoint[2] - comp[2];
	return diffX * diffX + diffY * diffY + diffZ * diffZ;
}
