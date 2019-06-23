#include "Vector3.h"
#include <cstring>

Vector3::Vector3(float x, float y, float z) {
	comp[0] = x;
	comp[1] = y;
	comp[2] = z;
}

Vector3::Vector3(const Vector3 &p2) {
	memcpy(comp, p2.comp, sizeof(float)*3);
}

Vector3& Vector3::operator=(const Vector3& other) {
	if (this != &other) {
		memcpy(comp, other.comp, sizeof(float)*3);
	}
	return *this;
}

float &Vector3::operator[] (int index) {
	return comp[index];	
}

float Vector3::operator[] (int index) const {
	return comp[index];	
}

Vector3& Vector3::operator+=(const Vector3 &v2) {
	comp[0] += v2.comp[0];
	comp[1] += v2.comp[1];
	comp[2] += v2.comp[2];
	return *this;
}

Vector3& Vector3::operator-=(const Vector3 &v2) {
	comp[0] -= v2.comp[0];
	comp[1] -= v2.comp[1];
	comp[2] -= v2.comp[2];
	return *this;
}

float Vector3::operator*=(const Vector3 &v2) {
	return (comp[0]*v2.comp[0] + comp[1]*v2.comp[1]
		+ comp[2]*v2.comp[2]);
}

Vector3& Vector3::operator*=(float t) {
	comp[0] *= t;
	comp[1] *= t;
	comp[2] *= t;
	return *this;
}

Vector3& Vector3::operator/=(float t) {
	comp[0] /= t;
	comp[1] /= t;
	comp[2] /= t;
	return *this;
}

Vector3 Vector3::operator+(const Vector3 &v2) {
	Vector3 summedVector(comp[0] + v2.comp[0],
		comp[1] + v2.comp[1], comp[2] + v2.comp[2]);
	return summedVector;
}

Vector3 Vector3::operator-(const Vector3 &v2) {
	Vector3 subbedVector(comp[0] - v2.comp[0],
		comp[1] - v2.comp[1], comp[2] - v2.comp[2]);
	return subbedVector;
}

float Vector3::operator*(const Vector3 &v2) {
	return (comp[0]*v2.comp[0] + comp[1]*v2.comp[1]
		+ comp[2]*v2.comp[2]);
}

