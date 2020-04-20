#include "Point3.h"
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

float Point3::GetDistanceSquared(
	const Point3& otherPoint) const {
	float diffX = otherPoint[0] - comp[0];
	float diffY = otherPoint[1] - comp[1];
	float diffZ = otherPoint[2] - comp[2];
	return diffX * diffX + diffY * diffY + diffZ*diffZ;
}
