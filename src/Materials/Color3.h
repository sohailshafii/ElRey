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

	Color3 operator^(float power) const;
	Color3& operator^=(float power);
	// TODO: rvalue stuff

	Color3 operator+(const Color3& p2) const;
	Color3& operator+=(const Color3& p2);
	
	Color3 operator*(const Color3& p2) const;
	Color3& operator*=(const Color3& p2);

	Color3 operator/(float divisor) const;
	Color3& operator/=(float divisor);

	Color3 operator*(float mult) const;
	Color3& operator*=(float mult);

	float &operator[] (int index);
	float operator[] (int index) const;

	static Color3 White() {
		return white;
	}

	static Color3 Black() {
		return black;
	}

	static Color3 Red() {
		return red;
	}

	static Color3 Green() {
		return green;
	}

	static Color3 Blue() {
		return blue;
	}


private:
	float comp[3];
	
	static Color3 white;
	static Color3 black;
	static Color3 red;
	static Color3 green;
	static Color3 blue;
};
