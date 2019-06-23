#pragma once

class Vector3 {
public:
	Vector3() {}
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

	inline Vector3 operator+(const Vector3 &v2);
	inline Vector3 operator-(const Vector3 &v2);
	inline float operator*(const Vector3 &v2);

private:
	float comp[3];
};
