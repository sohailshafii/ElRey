#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>

class Vec3 {
public:
	Vec3() {}
	Vec3(float x, float y, float z) { m[0] = x; m[1] = y; m[2] = z; }
	inline float x() const { return m[0]; }
	inline float y() const { return m[1]; }
	inline float z() const { return m[2]; }
	inline float r() const { return m[0]; }
	inline float g() const { return m[1]; }
	inline float b() const { return m[2]; }

	inline const Vec3& operator+() const { return *this; }
	inline Vec3 operator-() const { return Vec3(-m[0], -m[1], -m[2]); }
	inline float operator[](int i) const { return m[i]; }
	inline float& operator[](int i) { return m[i]; }

	inline Vec3& operator+=(const Vec3 &v2);
	inline Vec3& operator-=(const Vec3 &v2);
	inline Vec3& operator*=(const Vec3 &v2);
	inline Vec3& operator/=(const Vec3 &v2);

	inline Vec3& operator*=(const float t);
	inline Vec3& operator/=(const float t);

	inline float length() const {
		return sqrt(m[0]*m[0] + m[1]*m[1] + m[2]*m[2]);
	}
	inline float squaredLength() const {
		return m[0]*m[0] + m[1]*m[1] + m[2]*m[2];
	}
	inline void makeUnitVector();

	float m[3];
};

inline std::istream& operator>>(std::istream &is, Vec3 &t) {
	is >> t.m[0] >> t.m[1] >> t.m[2];
	return is;
}

inline std::ostream& operator>>(std::ostream &os, Vec3 &t) {
	os << t.m[0] << " " << t.m[1] << " " << t.m[2];
	return os;
}

inline void Vec3::makeUnitVector() {
	float k = 1.0 / sqrt(m[0]*m[0] + m[1]*m[1] + m[2]*m[2]);
	m[0] *= k; m[1] *= k; m[2] *= k;
}

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.m[0] + v2.m[0], v1.m[1] + v2.m[1],
		v1.m[2] + v2.m[2]);
}

inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.m[0] - v2.m[0], v1.m[1] - v2.m[1],
		v1.m[2] - v2.m[2]);
}

inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.m[0] * v2.m[0], v1.m[1] * v2.m[1],
		v1.m[2] * v2.m[2]);
}

inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.m[0] / v2.m[0], v1.m[1] / v2.m[1],
		v1.m[2] / v2.m[2]);
}

inline Vec3 operator*(float t, const Vec3 &v) {
	return Vec3(t*v.m[0], t*v.m[1], t*v.m[2]);
}

inline Vec3 operator/(const Vec3 &v, float t) {
	return Vec3(v.m[0]/t, v.m[1]/t, v.m[2]/t);
}

inline Vec3 operator*(const Vec3 &v, float t) {
	return Vec3(v.m[0]*t, v.m[1]*t, v.m[2]*t);
}

inline float dot(const Vec3 &v1, const Vec3 &v2) {
	return v1.m[0]*v2.m[0] + v1.m[1]*v2.m[1] + v1.m[2]*v2.m[2];
}

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.m[1]*v2.m[2] - v1.m[2]*v2.m[1],
		-v1.m[0]*v2.m[2] + v1.m[2]*v2.m[0],
		v1.m[0]*v2.m[1] - v1.m[1]*v2.m[0]);
}

inline Vec3& Vec3::operator+=(const Vec3 &v2) {
	m[0] += v2.m[0];
	m[1] += v2.m[1];
	m[2] += v2.m[2];
	return *this;
}

inline Vec3& Vec3::operator-=(const Vec3 &v2) {
	m[0] -= v2.m[0];
	m[1] -= v2.m[1];
	m[2] -= v2.m[2];
	return *this;
}

inline Vec3& Vec3::operator*=(const Vec3 &v2) {
	m[0] *= v2.m[0];
	m[1] *= v2.m[1];
	m[2] *= v2.m[2];
	return *this;
}

inline Vec3& Vec3::operator/=(const Vec3 &v2) {
	m[0] /= v2.m[0];
	m[1] /= v2.m[1];
	m[2] /= v2.m[2];
	return *this;
}

inline Vec3& Vec3::operator*=(const float t) {
	m[0] *= t;
	m[1] *= t;
	m[2] *= t;
	return *this;
}

inline Vec3& Vec3::operator/=(const float t) {
	float k = 1.0/t;
	m[0] *= k;
	m[1] *= k;
	m[2] *= k;
	return *this;
}

inline Vec3 unitVector(const Vec3& v) {
	return v/v.length();
}

