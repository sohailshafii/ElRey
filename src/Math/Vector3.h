
class Vector3 {
public:
	Vector3(float x, float y, float z);

	Vector3(const Vector3 &p2);
	Vector3& operator=(const Vector3& other);

	int &operator[] (int index);
	int operator[] (int index) const;

private:
	float comp[3];
};
