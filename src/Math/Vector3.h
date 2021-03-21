#pragma once

#include <cmath>
#include "CommonMath.h"

class Point3;
class Point4;

class Vector3 {
public:
	Vector3() {
		comp[0] = 0.0f;
		comp[1] = 0.0f;
		comp[2] = 0.0f;
	}
	Vector3(float x, float y, float z);

	Vector3(const Vector3 &p2);
	Vector3& operator=(const Vector3& other);

	inline float &operator[] (int index);
	inline float operator[] (int index) const;

	inline Vector3& operator+=(const Vector3 &v2);
	inline Vector3& operator-=(const Vector3 &v2);
	inline float operator*=(const Vector3 &v2);

	inline Vector3& operator*=(float t);
	inline Vector3& operator/=(float t);

	float operator*(const Point3& point) const;
	Point3 operator+(const Point3& point) const;

	float operator*(const Point4 &point) const;
	Point4 operator+(const Point4 &point) const;

	inline Vector3 operator+(const Vector3 &v2) const;
	inline Vector3 operator-(const Vector3 &v2) const;
	inline float operator*(const Vector3 &v2) const;

	inline Vector3 operator*(float t) const;
	
	inline Vector3 operator^(const Vector3& v2) const;

	inline Vector3 operator-() const;

	inline void Normalize();
	inline float Norm() const;
	inline float NormSqr() const;
	
	inline Vector3 Normalized() const;

	static Vector3 Zero() {
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	static Vector3 Right() {
		return Vector3(1.0f, 0.0f, 0.0f);
	}

	static Vector3 Up() {
		return Vector3(0.0f, 1.0f, 0.0f);
	}

	static Vector3 Forward() {
		return Vector3(0.0f, 0.0f, 1.0f);
	}
	
	bool IsZeroVector() const {
		return fabs(comp[0]) < EPSILON &&
			fabs(comp[1]) < EPSILON &&
			fabs(comp[1]) < EPSILON;
	}

private:
	float comp[3];
};

inline float &Vector3::operator[] (int index) {
	return comp[index];
}

inline float Vector3::operator[] (int index) const {
	return comp[index];
}

inline Vector3& Vector3::operator+=(const Vector3 &v2) {
	comp[0] += v2.comp[0];
	comp[1] += v2.comp[1];
	comp[2] += v2.comp[2];
	return *this;
}

inline Vector3& Vector3::operator-=(const Vector3 &v2) {
	comp[0] -= v2.comp[0];
	comp[1] -= v2.comp[1];
	comp[2] -= v2.comp[2];
	return *this;
}

inline float Vector3::operator*=(const Vector3 &v2) {
	return (comp[0]*v2.comp[0] + comp[1]*v2.comp[1]
		+ comp[2]*v2.comp[2]);
}

inline Vector3& Vector3::operator*=(float t) {
	comp[0] *= t;
	comp[1] *= t;
	comp[2] *= t;
	return *this;
}

inline Vector3& Vector3::operator/=(float t) {
	comp[0] /= t;
	comp[1] /= t;
	comp[2] /= t;
	return *this;
}

inline Vector3 Vector3::operator+(const Vector3 &v2) const {
	Vector3 summedVector(comp[0] + v2.comp[0],
		comp[1] + v2.comp[1], comp[2] + v2.comp[2]);
	return summedVector;
}

inline Vector3 Vector3::operator-(const Vector3 &v2) const {
	Vector3 subbedVector(comp[0] - v2.comp[0],
		comp[1] - v2.comp[1], comp[2] - v2.comp[2]);
	return subbedVector;
}

inline float Vector3::operator*(const Vector3 &v2) const {
	return (comp[0]*v2.comp[0] + comp[1]*v2.comp[1]
		+ comp[2]*v2.comp[2]);
}

inline Vector3 Vector3::operator^(const Vector3& v2) const {
	return Vector3(comp[1] * v2[2] - comp[2] * v2[1],
		comp[2] * v2[0] - comp[0] * v2[2],
		comp[0] * v2[1] - comp[1] * v2[0]);
}

inline Vector3 Vector3::operator*(float t) const {
	return Vector3(comp[0]*t, comp[1]*t, comp[2]*t);
}

inline Vector3 Vector3::operator-() const {
	Vector3 negatedVector(-comp[0], -comp[1],
		-comp[2]);
	return negatedVector;
}

inline void Vector3::Normalize() {
	float vectorNorm = Norm();
	comp[0] /= vectorNorm;
	comp[1] /= vectorNorm;
	comp[2] /= vectorNorm;
}

inline Vector3 Vector3::Normalized() const {
	Vector3 normalizedVec(comp[0], comp[1], comp[2]);
	normalizedVec.Normalize();
	return normalizedVec;
}

inline float Vector3::Norm() const {
	return sqrt(comp[0]*comp[0] + comp[1]*comp[1] +
		comp[2]*comp[2]);
}

inline float Vector3::NormSqr() const {
	return (comp[0]*comp[0] + comp[1]*comp[1] +
		comp[2]*comp[2]);
}
