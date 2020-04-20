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

	inline float operator*(const Vector3 &vector) const;
	inline Point3 operator+(const Vector3 &vector) const;
	inline Point3 operator-(const Vector3 &vector) const;

	inline Point3 operator+(const Point3&p2) const;

	inline Vector3 operator-(const Point3&p2) const;

	inline Point3 operator-() const;

	inline Point3 operator*(float t) const;

	float GetDistanceSquared(const Point3& otherPoint) const;

	static Point3 Zero() {
		return Point3(0.0f, 0.0f, 0.0f);
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

inline float Point3::operator*(const Vector3 &vector) const {
	return (comp[0]*vector[0] + comp[1]*vector[1]
		+ comp[2]*vector[2]);
}

inline Point3 Point3::operator+(const Vector3 &vector) const {
	return Point3(comp[0]+vector[0], comp[1]+vector[1],
		comp[2]+vector[2]);
}

inline Point3 Point3::operator-(const Vector3 &vector) const {
	return Point3(comp[0]-vector[0], comp[1]-vector[1],
		comp[2]-vector[2]);
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

inline Point3 Point3::operator*(float t) const {
	Point3 multPoint(t * comp[0], t * comp[1], t * comp[2]);
	return multPoint;
}
