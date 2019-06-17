#pragma once

class Vector3 {
public:
	Vector3() {}
	Vector3(float x, float y, float z);

	Vector3(const Vector3 &p2);
	Vector3& operator=(const Vector3& other);
	// TODO: r-value stuff

	float &operator[] (int index);
	float operator[] (int index) const;

private:
	float comp[3];
};
