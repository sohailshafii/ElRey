#pragma once

class Vector3;

// TODO: write tests for this stuff
// row-order matrix
class Matrix {
public:
	Matrix(unsigned int numRows, unsigned int numColumns);
	// 4x4
	Matrix(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	~Matrix();

	Matrix(const Matrix &rhs);
	Matrix(Matrix &&rhs);
	Matrix& operator=(const Matrix& rhs);

	Matrix Transpose() const;
	void MakeIdentity();
	void FillWithZeros();
	void FillWithRandomValues(float min, float max);
	void Print() const;

	Matrix operator+(const Matrix& rhs) const;
	Matrix operator-(const Matrix& rhs) const;
	Matrix operator*(const Matrix& rhs) const;

	Matrix& operator+=(const Matrix& rhs);
	Matrix& operator-=(const Matrix& rhs);
	Matrix& operator*=(const Matrix& rhs);

	Matrix operator+(float scalar) const;
	Matrix operator-(float scalar) const;
	Matrix operator*(float scalar) const;
	Matrix operator/(float scalar) const;

	Matrix& operator+=(float scalar);
	Matrix& operator-=(float scalar);
	Matrix& operator*=(float scalar);
	Matrix& operator/=(float scalar);

	static Matrix Translate(const Vector3& translationVec);
	static Matrix Scale(const Vector3& scaleVec);
	// axis must be normalized!
	static Matrix Rotate(const Vector3& axis, float angle);

	int GetNumRows() const { return numRows; }
	int GetNumColumns() const { return numColumns; }

	inline float* GetRow(unsigned int rowIndex);
	inline float* operator[](unsigned int rowIndex);

	inline float& operator()(unsigned int row, unsigned int col);
	inline float operator()(unsigned int row, unsigned int col) const;

private:
	float *m;
	unsigned int numRows, numColumns, numElements;

	void AllocateAndCopyFrom(const Matrix& other);
};

inline float* Matrix::GetRow(unsigned int rowIndex) {
	return m + rowIndex*numColumns;
}

inline float* Matrix::operator[](unsigned int rowIndex) {
	return m + rowIndex*numColumns;
}

inline float& Matrix::operator()(unsigned int row,
	unsigned int col) {
	return m[row*numColumns + col];
}

inline float Matrix::operator()(unsigned int row,
	unsigned int col) const {
	return m[row*numColumns + col];
}
