#pragma once

class Color {
public:
	Color() {
		comp[0] = 0.0f;
		comp[1] = 0.0f;
		comp[2] = 0.0f;
		comp[3] = 0.0f;
	}
	
	Color(float r, float g, float b,
		float a) {
		comp[0] = r;
		comp[1] = g;
		comp[2] = b;
		comp[3] = a;
	}
	
	float GetRelativeLuminance() const {
		return 0.2126f*comp[0] + 0.7152f*comp[1] + 0.0722f*comp[2];
	}

	Color(const Color &p2);
	Color& operator=(const Color& other);

	Color operator^(float power);
	Color& operator^=(float power);
	// TODO: rvalue stuff

	Color operator+(const Color& p2) const;
	Color& operator+=(const Color& p2);
	
	Color operator*(const Color& p2) const;
	Color& operator*=(const Color& p2);

	Color operator/(float divisor) const;
	Color& operator/=(float divisor);

	Color operator*(float mult) const;
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
