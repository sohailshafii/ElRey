#include "Color.h"
#include <cstring>
#include <cmath>

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

Color Color::operator^(float power) {
	return Color(pow(comp[0], power), pow(comp[1], power),
		pow(comp[2], power), pow(comp[3], power));
}

Color& Color::operator^=(float power) {
	comp[0] = pow(comp[0], power);
	comp[1] = pow(comp[1], power);
	comp[2] = pow(comp[2], power);
	comp[3] = pow(comp[3], power);
	return *this;
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
