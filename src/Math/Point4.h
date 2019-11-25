#pragma once

#include "Math/Vector3.h"

// This class represents a homogenous point. We probably don't need
// this and can rely on the Point3 data type. But this exists in case
// we require it.
class Point4 {
public:
	Point4() {}
	Point4(float x, float y, float z);
	Point4(float x, float y, float z, float w);

	Point4(const Point4 &p2);
	Point4& operator=(const Point4& other);

	inline float &operator[] (int index);
	inline float operator[] (int index) const;

	inline Point4& operator+=(const Vector3 &vec);
	inline Point4& operator-=(const Vector3 &vec);

	inline Point4& operator*=(float t);
	inline Point4& operator/=(float t);

	float operator*(const Vector3 &vector) const;
	Point4 operator+(const Vector3 &vector) const;
	Point4 operator-(const Vector3 &vector) const;

	inline Point4 operator+(const Point4 &p2) const;

	inline Vector3 operator-(const Point4 &p2) const;

	inline Point4 operator-() const;

	static Point4 Zero() {
		return Point4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	static Point4 Up() {
		return Point4(0.0f, 1.0f, 0.0f, 1.0f);
	}

	static Point4 Right() {
		return Point4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	static Point4 Forward() {
		return Point4(0.0f, 0.0f, 1.0f, 1.0f);
	}

private:
	float comp[4];
};

inline float &Point4::operator[] (int index) {
	return comp[index];
}

inline float Point4::operator[] (int index) const {
	return comp[index];
}

inline Point4& Point4::operator+=(const Vector3 &vec) {
	comp[0] += vec[0];
	comp[1] += vec[1];
	comp[2] += vec[2];
	return *this;
}

inline Point4& Point4::operator-=(const Vector3 &vec) {
	comp[0] -= vec[0];
	comp[1] -= vec[1];
	comp[2] -= vec[2];
	return *this;
}

inline Point4& Point4::operator*=(float t) {
	comp[0] *= t;
	comp[1] *= t;
	comp[2] *= t;
	return *this;
}

inline Point4& Point4::operator/=(float t) {
	comp[0] /= t;
	comp[1] /= t;
	comp[2] /= t;
	return *this;
}

inline Vector3 Point4::operator-(const Point4 &p2) const {
	return Vector3(comp[0] - p2[0], comp[1] - p2[1],
		comp[2] - p2[2]);
}

inline Point4 Point4::operator+(const Point4 &p2) const {
	return Point4(comp[0] + p2[0], comp[1] + p2[1],
		comp[2] + p2[2], comp[3]);
}

inline Point4 Point4::operator-() const {
	Point4 negatedPoint(-comp[0], -comp[1],
		-comp[2], comp[3]);
	return negatedPoint;
}
