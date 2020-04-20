#include "Point2.h"
#include <cstring>

Point2::Point2(float x, float y) {
	comp[0] = x;
	comp[1] = y;
}

Point2::Point2(const Point2 &p2) {
	comp[0] = p2[0];
	comp[1] = p2[1];
}

Point2& Point2::operator=(const Point2& p2) {
	if (this != &p2) {
		memcpy(comp, p2.comp, sizeof(float) * 2);
	}
	return *this;
}

float Point2::GetDistanceSquared(
	const Point2& otherPoint) const {
	float diffX = otherPoint[0] - comp[0];
	float diffY = otherPoint[1] - comp[1];
	return diffX * diffX + diffY * diffY;
}
