#pragma once

class Color3 {
public:
	Color3() {
	}
	Color3(float r, float g, float b) {
		comp[0] = r;
		comp[1] = g;
		comp[2] = b;
	}

	Color3(const Color3 &p2);
	Color3& operator=(const Color3& other);

	Color3 operator^(float power);
	Color3& operator^=(float power);
	// TODO: rvalue stuff

	Color3 operator+(const Color3& p2);
	Color3& operator+=(const Color3& p2);

	Color3 operator/(float divisor);
	Color3& operator/=(float divisor);

	Color3 operator*(float mult) const;
	Color3& operator*=(float mult);

	float &operator[] (int index);
	float operator[] (int index) const;

	static Color3 White();
	static Color3 Black();
	static Color3 Red();
	static Color3 Green();
	static Color3 Blue();

private:
	float comp[3];
};
