#pragma once

class Color {
public:
	Color() {
		
	}
	Color(float r, float g, float b,
		float a) {
		comp[0] = r;
		comp[1] = g;
		comp[2] = b;
		comp[3] = a;
	}

	Color(const Color &p2);
	Color& operator=(const Color& other);

	Color operator^(float power);
	Color& operator^=(float power);
	// TODO: rvalue stuff

	Color operator+(const Color& p2);
	Color& operator+=(const Color& p2);

	Color operator/(float divisor);
	Color& operator/=(float divisor);

	Color operator*(float mult);
	Color& operator*=(float mult);

	float &operator[] (int index);
	float operator[] (int index) const;

	static Color White();
	static Color Black();
	static Color Red();
	static Color Green();
	static Color Blue();

private:
	float comp[4];
};
