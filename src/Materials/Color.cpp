#include "Color.h"

Color::Color(const Color &p2) {
	memcpy(comp, p2.comp, sizeof(float)*4);
}

Color& Color::operator=(const Color& other) {
	if (this != &other) {
		memcpy(comp, other.comp, sizeof(float)*4);
	}
	return *this;
}

int &Color::operator[] (int index) {
	return comp[index];
}

int Color::operator[] (int index) const {
	return comp[index];
}
