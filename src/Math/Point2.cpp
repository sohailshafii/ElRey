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
	memcpy(comp, p2.comp, sizeof(float)*2);
}
