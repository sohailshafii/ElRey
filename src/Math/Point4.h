#pragma once

class Vector3;

class Point4 {
public:
	Point4() {}
	Point4(float x, float y, float z, float w);

	Point4(const Point4 &p2);
	Point4& operator=(const Point4& other);

	inline float &operator[] (int index);
	inline float operator[] (int index) const;

	inline Point4& operator+=(const Point4 &p2);
	inline Point4& operator-=(const Point4 &p2);

	inline Point4& operator*=(float t);
	inline Point4& operator/=(float t);

	float operator*(const Vector3 &vector) const;

	inline Point4 operator+(const Point4 &p2) const;
	inline Point4 operator-(const Point4 &p2) const;

	inline Point4 operator-() const;

private:
	float comp[4];
};

inline float &Point4::operator[] (int index) {
	return comp[index];
}

inline float Point4::operator[] (int index) const {
	return comp[index];
}

inline Point4& Point4::operator+=(const Point4 &p2) {
	comp[0] += p2.comp[0];
	comp[1] += p2.comp[1];
	comp[2] += p2.comp[2];
	return *this;
}

inline Point4& Point4::operator-=(const Point4 &p2) {
	comp[0] -= p2.comp[0];
	comp[1] -= p2.comp[1];
	comp[2] -= p2.comp[2];
	return *this;
}

inline Point4& Point4::operator*=(float t) {
	comp[0] *= t;
	comp[1] *= t;
	comp[2] *= t;
	return *this;
}

inline Point4& Point4::operator/=(float t) {
	comp[0] /= t;
	comp[1] /= t;
	comp[2] /= t;
	return *this;
}

inline Point4 Point4::operator+(const Point4 &p2) const {
	Point4 summedPoint(comp[0] + p2.comp[0],
		comp[1] + p2.comp[1], comp[2] + p2.comp[2],
		comp[3] + p2.comp[3]);
	return summedPoint;
}

inline Point4 Point4::operator-(const Point4 &p2) const {
	Point4 subbedPoint(comp[0] - p2.comp[0],
		comp[1] - p2.comp[1], comp[2] - p2.comp[2],
		comp[3] - p2.comp[3]);
	return subbedPoint;
}

inline Point4 Point4::operator-() const {
	Point4 negatedPoint(-comp[0], -comp[1],
		-comp[2], -comp[3]);
	return negatedPoint;
}
