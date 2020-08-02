//https://towardsdatascience.com/how-to-build-your-own-neural-network-from-scratch-in-python-68998a08e4f6

#include "Matrix.h"
#include "CommonMath.h"
#include "Math/Vector3.h"
#include <cstring>
#include <iostream>

Matrix::Matrix(unsigned int numRows, unsigned int numColumns) {
	this->numRows = numRows;
	this->numColumns = numColumns;

	numElements = numRows*numColumns;
	m = new float[numElements];
	MakeIdentity();
}

Matrix::Matrix(float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33) {

	this->numRows = 4;
	this->numColumns = 4;

	numElements = numRows * numColumns;
	m = new float[numElements];
	m[0] = m00; m[1] = m01; m[2] = m02; m[3] = m03;
	m[4] = m10; m[5] = m11; m[6] = m12; m[7] = m13;
	m[8] = m20; m[9] = m21; m[10] = m22; m[11] = m23;
	m[12] = m30; m[13] = m31; m[14] = m32; m[15] = m33;
}

Matrix::~Matrix() {
	if (m != nullptr) {
		delete[] m;
	}
}

Matrix::Matrix(Matrix const & rhs) {
	AllocateAndCopyFrom(rhs);
}

Matrix::Matrix(Matrix &&rhs): m(rhs.m),
	numRows(rhs.numRows), numColumns(rhs.numColumns),
	numElements(rhs.numElements) {
}

Matrix& Matrix::operator=(Matrix const & rhs) {
	if (&rhs != this) {
		AllocateAndCopyFrom(rhs);
	}

	return *this;
}

Matrix Matrix::Transpose() const {
	auto transposedNumRows = this->numColumns;
	auto transposedNumColumns = this->numRows;
	Matrix transposed(transposedNumRows,
		transposedNumColumns);

	for (unsigned int row = 0; row < transposedNumRows; row++) {
		for (unsigned int column = 0; column < transposedNumColumns; column++) {
			transposed[row][column] = (*this)(column, row);
		}
	}
	return transposed;
}

void Matrix::MakeIdentity() {
	FillWithZeros();
	for (unsigned int rowIndex = 0, oneDimIndex = 0; rowIndex < numRows;
		rowIndex++) {
		for (unsigned int colIndex = 0; colIndex < numColumns;
			colIndex++, oneDimIndex++) {
			if (rowIndex == colIndex) {
				m[oneDimIndex] = 1.0f;
			}
		}
	}
}

void Matrix::FillWithZeros() {
	for (unsigned int elementIndex = 0; elementIndex <
		numElements; elementIndex++) {
		m[elementIndex] = 0.0f;
	}
}

void Matrix::FillWithRandomValues(float min, float max) {
	for (unsigned int elementIndex = 0; elementIndex <
		numElements; elementIndex++) {
		m[elementIndex] = CommonMath::RandomUnitValue();
	}
}

void Matrix::Print() const {
	for (unsigned int rowIndex = 0, oneDimIndex = 0; rowIndex < numRows;
		rowIndex++) {
		for (unsigned int colIndex = 0; colIndex < numColumns;
			colIndex++, oneDimIndex++) {
			std::cout << m[oneDimIndex] << ", ";
		}
		std::cout << std::endl;
	}
}

void Matrix::AllocateAndCopyFrom(const Matrix& other) {
	this->numRows = other.numRows;
	this->numColumns = other.numColumns;

	numElements = numRows*numColumns;
	m = new float[numElements];
	memcpy(m, other.m, numElements * sizeof(float));
}

Matrix Matrix::operator+(const Matrix& rhs) const {
	Matrix sum(rhs.numRows, rhs.numColumns);

	for (unsigned int elementIndex = 0; elementIndex < numElements;
		elementIndex++) {
		sum.m[elementIndex] = this->m[elementIndex]
			+ rhs.m[elementIndex];
	}

	return sum;
}

Matrix Matrix::operator-(const Matrix& rhs) const {
	Matrix diff(rhs.numRows, rhs.numColumns);

	for (unsigned int elementIndex = 0; elementIndex < numElements;
		elementIndex++) {
		diff.m[elementIndex] = this->m[elementIndex]
			- rhs.m[elementIndex];
	}

	return diff;
}

Matrix Matrix::operator*(const Matrix& rhs) const {
	unsigned int resultRows = this->numRows,
		resultCols = rhs.numColumns;
	Matrix product(resultRows, resultCols);
	product.FillWithZeros();

	if (this->numColumns != rhs.numRows) {
		std::cerr << "Can't multiply a (" << numRows
			<< " x " << numColumns << ") matrix "
			<< "with a (" << rhs.numRows <<
			" x " << rhs.numColumns << ") matrix.\n";
	}
	else {
		for (unsigned int row = 0; row < resultRows; row++) {
			auto currRow = product[row];

			for (unsigned int column = 0; column < resultCols;
				column++) {
				for (unsigned int multIndex = 0; multIndex <
					numColumns; multIndex++) {
					currRow[column] += ((*this)(row, multIndex) *
						rhs(multIndex, column));
				}
			}
		}
	}

	return product;
}

Matrix& Matrix::operator+=(const Matrix& rhs) {
	for (unsigned int elementIndex = 0; elementIndex < numElements;
		elementIndex++) {
		m[elementIndex] += rhs.m[elementIndex];
	}
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& rhs) {
	for (unsigned int elementIndex = 0; elementIndex < numElements;
		elementIndex++) {
		m[elementIndex] -= rhs.m[elementIndex];
	}
	return *this;
}

Matrix& Matrix::operator*=(const Matrix& rhs) {
	if (this->numRows != rhs.numRows ||
		this->numColumns != rhs.numColumns) {
		std::cerr << "Can't self multiply a (" << numRows
			<< " x " << numColumns << ") matrix "
			<< "with a (" << rhs.numRows <<
			" x " << rhs.numColumns << ") matrix.\n";
	}
	else {
		*this = *this*rhs;
	}
	return *this;
}

Matrix Matrix::operator+(float scalar) const {
	Matrix sum(*this);

	for (unsigned int elementIndex = 0; elementIndex < numElements;
		elementIndex++) {
		sum.m[elementIndex] = this->m[elementIndex]
			+ scalar;
	}

	return sum;
}

Matrix Matrix::operator-(float scalar) const {
	Matrix diff(*this);

	for (unsigned int elementIndex = 0; elementIndex < numElements;
		elementIndex++) {
		diff.m[elementIndex] = this->m[elementIndex]
			- scalar;
	}

	return diff;
}

Matrix Matrix::operator*(float scalar)  const {
	Matrix product(*this);

	for (unsigned int elementIndex = 0; elementIndex < numElements;
		elementIndex++) {
		product.m[elementIndex] = this->m[elementIndex]
			* scalar;
	}

	return product;
}

Matrix Matrix::operator/(float scalar) const {
	Matrix div(*this);

	for (unsigned int elementIndex = 0; elementIndex < numElements;
		elementIndex++) {
		div.m[elementIndex] = this->m[elementIndex]
			/ scalar;
	}

	return div;
}

Matrix& Matrix::operator+=(float scalar) {
	for (unsigned int elementIndex = 0; elementIndex < numElements;
		elementIndex++) {
		m[elementIndex] += scalar;
	}

	return *this;
}

Matrix& Matrix::operator-=(float scalar) {
	for (unsigned int elementIndex = 0; elementIndex < numElements;
		elementIndex++) {
		m[elementIndex] -= scalar;
	}

	return *this;
}

Matrix& Matrix::operator*=(float scalar) {
	for (unsigned int elementIndex = 0; elementIndex < numElements;
		elementIndex++) {
		m[elementIndex] *= scalar;
	}

	return *this;
}

// yeah, ignores dimensions of matrix and doesn't check them
// assumes 3d point as a w-coordinate of 1
Point3 Matrix::operator*(const Point3& rhs) const {
	return Point3(m[0]*rhs[0] + m[1]*rhs[2] + m[2]*rhs[2] + m[3],
				  m[4]*rhs[0] + m[5]*rhs[2] + m[6]*rhs[3] + m[7],
				  m[8]*rhs[0] + m[9]*rhs[2] + m[10]*rhs[3] + m[11]);
}

Point4 Matrix::operator*(const Point4& rhs) const {
	return Point4(m[0]*rhs[0] + m[1]*rhs[2] + m[2]*rhs[2] + m[3]*rhs[3],
				  m[4]*rhs[0] + m[5]*rhs[2] + m[6]*rhs[3] + m[7]*rhs[3],
				  m[8]*rhs[0] + m[9]*rhs[2] + m[10]*rhs[3] + m[11]*rhs[3]);
}

// assumes vector has w-coordinate of 0 for no translation
Vector3 Matrix::operator*(const Vector3& rhs) const {
	return Vector3(m[0]*rhs[0] + m[1]*rhs[2] + m[2]*rhs[2],
				  m[4]*rhs[0] + m[5]*rhs[2] + m[6]*rhs[3],
				  m[8]*rhs[0] + m[9]*rhs[2] + m[10]*rhs[3]);
}

Matrix& Matrix::operator/=(float scalar) {
	for (unsigned int elementIndex = 0; elementIndex < numElements;
		elementIndex++) {
		m[elementIndex] /= scalar;
	}

	return *this;
}

Matrix Matrix::TranslationMatrix(const Vector3& translationVec) {
	return Matrix(
		1.0f, 0.0f, 0.0f, translationVec[0],
		0.0f, 1.0f, 0.0f, translationVec[1],
		0.0f, 0.0f, 1.0f, translationVec[2],
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix Matrix::InvTranslationMatrix(const Vector3& translationVec) {
	return Matrix(
		1.0f, 0.0f, 0.0f,-translationVec[0],
		0.0f, 1.0f, 0.0f,-translationVec[1],
		0.0f, 0.0f, 1.0f,-translationVec[2],
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix Matrix::ScaleMatrix(const Vector3& scaleVec) {
	return Matrix(
		scaleVec[0], 0.0f, 0.0f, 0.0f,
		0.0f, scaleVec[1], 0.0f, 0.0f,
		0.0f, 0.0f, scaleVec[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix Matrix::InvScaleMatrix(const Vector3& scaleVec) {
	return Matrix(
		1.0f/scaleVec[0], 0.0f, 0.0f, 0.0f,
		0.0f,1.0f/scaleVec[1], 0.0f, 0.0f,
		0.0f, 0.0f,1.0f/scaleVec[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix Matrix::RotationMatrix(const Vector3& axis, float angleDegrees) {
	float cosAngle = cos(angleDegrees*DEG_2_RAD);
	float sinAngle = sin(angleDegrees*DEG_2_RAD);
	float oneMinusCos = 1.0f - cosAngle;
	float uxUy = axis[0] * axis[1];
	float uzUy = axis[2] * axis[1];
	float uzUx = axis[2] * axis[0];
	return Matrix(
		cosAngle + axis[0] * axis[0] * oneMinusCos,
		uxUy * oneMinusCos - axis[2] * sinAngle,
		uzUx * oneMinusCos + axis[1] * sinAngle,
		0.0f,

		uxUy * oneMinusCos + axis[2] * sinAngle,
		cosAngle + axis[1]*axis[1]*oneMinusCos,
		uzUy*oneMinusCos - axis[0]*sinAngle,
		0.0f,
		
		uzUx*oneMinusCos - axis[1]*sinAngle,
		uzUy*oneMinusCos + axis[0]*sinAngle,
		cosAngle + axis[2]*axis[2]*oneMinusCos,
		0.0f,
		
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix Matrix::InvRotationMatrix(const Vector3& axis, float angleDegrees) {
	float cosAngle = cos(-angleDegrees*DEG_2_RAD);
	float sinAngle = sin(-angleDegrees*DEG_2_RAD);
	float oneMinusCos = 1.0f - cosAngle;
	float uxUy = axis[0] * axis[1];
	float uzUy = axis[2] * axis[1];
	float uzUx = axis[2] * axis[0];
	return Matrix(
		cosAngle + axis[0] * axis[0] * oneMinusCos,
		uxUy * oneMinusCos - axis[2] * sinAngle,
		uzUx * oneMinusCos + axis[1] * sinAngle,
		0.0f,

		uxUy * oneMinusCos + axis[2] * sinAngle,
		cosAngle + axis[1]*axis[1]*oneMinusCos,
		uzUy*oneMinusCos - axis[0]*sinAngle,
		0.0f,
		
		uzUx*oneMinusCos - axis[1]*sinAngle,
		uzUy*oneMinusCos + axis[0]*sinAngle,
		cosAngle + axis[2]*axis[2]*oneMinusCos,
		0.0f,
		
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix Matrix::RotationMatrixX(float angleDegrees) {
	float cosAngle = cos(angleDegrees*DEG_2_RAD);
	float sinAngle = sin(angleDegrees*DEG_2_RAD);
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosAngle, -sinAngle, 0.0f,
		0.0f, sinAngle, cosAngle, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix Matrix::InvRotationMatrixX(float angleDegrees) {
	float cosAngle = cos(-angleDegrees*DEG_2_RAD);
	float sinAngle = sin(-angleDegrees*DEG_2_RAD);
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosAngle, -sinAngle, 0.0f,
		0.0f, sinAngle, cosAngle, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix Matrix::RotationMatrixY(float angleDegrees) {
	float cosAngle = cos(angleDegrees*DEG_2_RAD);
	float sinAngle = sin(angleDegrees*DEG_2_RAD);
	return Matrix(
		cosAngle, 0.0f, sinAngle, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sinAngle, 0.0f, cosAngle, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix Matrix::InvRotationMatrixY(float angleDegrees) {
	float cosAngle = cos(-angleDegrees*DEG_2_RAD);
	float sinAngle = sin(-angleDegrees*DEG_2_RAD);
	return Matrix(
		cosAngle, 0.0f, sinAngle, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sinAngle, 0.0f, cosAngle, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix Matrix::RotationMatrixZ(float angleDegrees) {
	float cosAngle = cos(angleDegrees*DEG_2_RAD);
	float sinAngle = sin(angleDegrees*DEG_2_RAD);
	return Matrix(
		cosAngle, -sinAngle, 0.0f, 0.0f,
		sinAngle, cosAngle, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix Matrix::InvRotationMatrixZ(float angleDegrees) {
	float cosAngle = cos(-angleDegrees*DEG_2_RAD);
	float sinAngle = sin(-angleDegrees*DEG_2_RAD);
	return Matrix(
		cosAngle, -sinAngle, 0.0f, 0.0f,
		sinAngle, cosAngle, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}
