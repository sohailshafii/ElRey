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

Color Color::Black() {
	return Color(0.0f, 0.0f, 0.0f, 1.0f);
}

Color Color::Red() {
	return Color(1.0f, 0.0f, 0.0f, 1.0f);
}

Color Color::Green() {
	return Color(0.0f, 1.0f, 0.0f, 1.0f);
}

Color Color::Blue() {
	return Color(0.0f, 0.0f, 1.0f, 1.0f);
}
