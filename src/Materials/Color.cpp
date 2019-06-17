#include "Color.h"
#include <cstring>

Color::Color(const Color &p2) {
	memcpy(comp, p2.comp, sizeof(float)*4);
}

Color& Color::operator=(const Color& other) {
	if (this != &other) {
		memcpy(comp, other.comp, sizeof(float)*4);
	}
	return *this;
}

float &Color::operator[] (int index) {
	return comp[index];
}

float Color::operator[] (int index) const {
	return comp[index];
}
