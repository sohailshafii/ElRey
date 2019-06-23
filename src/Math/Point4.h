#pragma once

class Point4 {
public:
	Point4() {}
	Point4(float x, float y, float z, float w);

	Point4(const Point4 &p2);
	Point4& operator=(const Point4& other);

	float &operator[] (int index);
	float operator[] (int index) const;

private:
	float comp[4];
};
