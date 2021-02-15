#include "Color3.h"
#include <cstring>
#include <cmath>

Color3::Color3(const Color3 &p2) {
	memcpy(comp, p2.comp, sizeof(float)*3);
}

Color3& Color3::operator=(const Color3& other) {
	if (this != &other) {
		memcpy(comp, other.comp, sizeof(float)*3);
	}
	return *this;
}

float &Color3::operator[] (int index) {
	return comp[index];
}

float Color3::operator[] (int index) const {
	return comp[index];
}

Color3 Color3::operator^(float power) const {
	return Color3(pow(comp[0], power), pow(comp[1], power),
		pow(comp[2], power));
}

Color3& Color3::operator^=(float power) {
	comp[0] = pow(comp[0], power);
	comp[1] = pow(comp[1], power);
	comp[2] = pow(comp[2], power);
	return *this;
}

Color3 Color3::operator+(const Color3& p2) const {
	return Color3(comp[0] + p2[0], comp[1] + p2[1],
		comp[2] + p2[2]);
}

Color3& Color3::operator+=(const Color3& p2) {
	comp[0] += p2[0];
	comp[1] += p2[1];
	comp[2] += p2[2];
	return *this;
}

Color3 Color3::operator*(const Color3& p2) const {
	return Color3(comp[0] * p2[0], comp[1] * p2[1],
		comp[2] * p2[2]);
}

Color3& Color3::operator*=(const Color3& p2) {
	comp[0] *= p2[0];
	comp[1] *= p2[1];
	comp[2] *= p2[2];
	return *this;
}

Color3 Color3::operator/(float divisor) const {
	return Color3(comp[0]/divisor, comp[1]/divisor,
		comp[2] / divisor);
}

Color3& Color3::operator/=(float divisor) {
	comp[0] /= divisor;
	comp[1] /= divisor;
	comp[2] /= divisor;
	return *this;
}

Color3 Color3::operator*(float mult) const {
	return Color3(comp[0] * mult, comp[1] * mult,
		comp[2] * mult);
}

Color3& Color3::operator*=(float mult) {
	comp[0] *= mult;
	comp[1] *= mult;
	comp[2] *= mult;
	return *this;
}

Color3 Color3::White() {
	return Color3(1.0f, 1.0f, 1.0f);
}

Color3 Color3::Black() {
	return Color3(0.0f, 0.0f, 0.0f);
}

Color3 Color3::Red() {
	return Color3(1.0f, 0.0f, 0.0f);
}

Color3 Color3::Green() {
	return Color3(0.0f, 1.0f, 0.0f);
}

Color3 Color3::Blue() {
	return Color3(0.0f, 0.0f, 1.0f);
}
