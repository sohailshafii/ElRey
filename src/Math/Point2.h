#pragma once

class Point2 {
public:
	Point2() {}
	Point2(float x, float y);

	Point2(const Point2 &p2);
	Point2& operator=(const Point2& p2);

	inline float &operator[] (int index);
	inline float operator[] (int index) const;

	inline Point2& operator*=(float t);
	inline Point2& operator/=(float t);

	inline Point2 operator-() const;

private:
	float comp[2];
};

inline float &Point2::operator[] (int index) {
	return comp[index];
}

inline float Point2::operator[] (int index) const {
	return comp[index];
}

inline Point2& Point2::operator*=(float t) {
	comp[0] *= t;
	comp[1] *= t;
	return *this;
}

inline Point2& Point2::operator/=(float t) {
	comp[0] /= t;
	comp[1] /= t;
	return *this;
}

inline Point2 Point2::operator-() const {
	Point2 negatedPoint(-comp[0], -comp[1]);
	return negatedPoint;
}

