#pragma once

class Point2 {
public:
	Point2() { comp[0] = 0.0f; comp[1] = 0.0f; }
	Point2(float x, float y);

	Point2(const Point2 &p2);
	Point2& operator=(const Point2& p2);

	inline float &operator[] (int index);
	inline float operator[] (int index) const;

	inline Point2& operator*=(float t);
	inline Point2& operator/=(float t);

	inline Point2 operator-() const;

	inline Point2 operator*(float t) const;

	inline float GetDistanceSquared(
		const Point2& otherPoint) const;

	static Point2 Zero() {
		return Point2(0.0f, 0.0f);
	}

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

inline Point2 Point2::operator*(float t) const {
	Point2 multPoint(t*comp[0], t * comp[1]);
	return multPoint;
}

