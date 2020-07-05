#pragma once

class Color3 {
public:
	Color3() {
		comp[0] = 0.0f;
		comp[1] = 0.0f;
		comp[2] = 0.0f;
	}
	
	Color3(float r, float g, float b) {
		comp[0] = r;
		comp[1] = g;
		comp[2] = b;
	}
	
	float GetRelativeLuminance() const {
		return 0.2126f*comp[0] + 0.7152f*comp[1] + 0.0722f*comp[2];
	}

	Color3(const Color3 &p2);
	Color3& operator=(const Color3& other);

	Color3 operator^(float power);
	Color3& operator^=(float power);
	// TODO: rvalue stuff

	Color3 operator+(const Color3& p2);
	Color3& operator+=(const Color3& p2);
	
	Color3 operator*(const Color3& p2);
	Color3& operator*=(const Color3& p2);

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
