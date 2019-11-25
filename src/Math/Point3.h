#pragma once

#include "Math/Vector3.h"

class Point3 {
public:
	Point3() {}
	Point3(float x, float y, float z);

	Point3(const Point3&p2);
	Point3& operator=(const Point3& other);

	inline float &operator[] (int index);
	inline float operator[] (int index) const;

	inline Point3& operator+=(const Vector3 &vec);
	inline Point3& operator-=(const Vector3 &vec);

	inline Point3& operator*=(float t);
	inline Point3& operator/=(float t);

	float operator*(const Vector3 &vector) const;
	Point3 operator+(const Vector3 &vector) const;
	Point3 operator-(const Vector3 &vector) const;

	inline Point3 operator+(const Point3&p2) const;

	inline Vector3 operator-(const Point3&p2) const;

	inline Point3 operator-() const;

	static Point3 Zero() {
		return Point3(0.0f, 0.0f, 0.0f);
	}

	static Point3 Right() {
		return Point3(1.0f, 0.0f, 0.0f);
	}

	static Point3 Up() {
		return Point3(0.0f, 1.0f, 0.0f);
	}

	static Point3 Forward() {
		return Point3(0.0f, 0.0f, 1.0f);
	}

private:
	float comp[3];
};

inline float & Point3::operator[] (int index) {
	return comp[index];
}

inline float Point3::operator[] (int index) const {
	return comp[index];
}

inline Point3& Point3::operator+=(const Vector3 &vec) {
	comp[0] += vec[0];
	comp[1] += vec[1];
	comp[2] += vec[2];
	return *this;
}

inline Point3& Point3::operator-=(const Vector3 &vec) {
	comp[0] -= vec[0];
	comp[1] -= vec[1];
	comp[2] -= vec[2];
	return *this;
}

inline Point3& Point3::operator*=(float t) {
	comp[0] *= t;
	comp[1] *= t;
	comp[2] *= t;
	return *this;
}

inline Point3& Point3::operator/=(float t) {
	comp[0] /= t;
	comp[1] /= t;
	comp[2] /= t;
	return *this;
}

inline Vector3 Point3::operator-(const Point3&p2) const {
	return Vector3(comp[0] - p2[0], comp[1] - p2[1],
		comp[2] - p2[2]);
}

inline Point3 Point3::operator+(const Point3&p2) const {
	return Point3(comp[0] + p2[0], comp[1] + p2[1],
		comp[2] + p2[2]);
}

inline Point3 Point3::operator-() const {
	Point3 negatedPoint(-comp[0], -comp[1], -comp[2]);
	return negatedPoint;
}
