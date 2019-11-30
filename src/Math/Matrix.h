#pragma once

// TODO: write tests for this stuff
// row-order matrix
class Matrix {
public:
	Matrix(unsigned int numRows, unsigned int numColumns);
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

	int GetNumRows() const { return numRows; }
	int GetNumColumns() const { return numColumns; }

	float* GetRow(unsigned int rowIndex);
	float* operator[](unsigned int rowIndex);

	float& operator()(unsigned int row, unsigned int col);
	float operator()(unsigned int row, unsigned int col) const;

private:
	float *m;
	unsigned int numRows, numColumns, numElements;

	void AllocateAndCopyFrom(const Matrix& other);
};
