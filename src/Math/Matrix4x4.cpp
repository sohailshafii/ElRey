
#include "Matrix4x4.h"
#include "CommonMath.h"
#include <iostream>

Matrix4x4::Matrix4x4() {
	m = new float[16];
	MakeIdentity();
}

Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03,
					 float m10, float m11, float m12, float m13,
					 float m20, float m21, float m22, float m23,
					 float m30, float m31, float m32, float m33) {
	m = new float[16];
	m[0] = m00; m[1] = m01; m[2] = m02; m[3] = m03;
	m[4] = m10; m[5] = m11; m[6] = m12; m[7] = m13;
	m[8] = m20; m[9] = m21; m[10] = m22; m[11] = m23;
	m[12] = m30; m[13] = m31; m[14] = m32; m[15] = m33;
}

Matrix4x4::Matrix4x4(Matrix4x4 const & rhs) {
	AllocateAndCopyFrom(rhs);
}

Matrix4x4::Matrix4x4(Matrix4x4 &&rhs): m(rhs.m) {
	rhs.m = nullptr;
}

Matrix4x4::~Matrix4x4() {
	if (m != nullptr) {
		delete[] m;
	}
}

Matrix4x4& Matrix4x4::operator=(Matrix4x4 const & rhs) {
	if (&rhs != this) {
		AllocateAndCopyFrom(rhs);
	}

	return *this;
}

void Matrix4x4::AllocateAndCopyFrom(const Matrix4x4& other) {
	if (m == nullptr) {
		m = new float[16];
	}
	memcpy(m, other.m, 16 * sizeof(float));
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& rhs) const {
	Matrix4x4 sum;
	sum.FillWithZeros();

	for (unsigned int elementIndex = 0; elementIndex < 16;
		elementIndex++) {
		sum.m[elementIndex] = this->m[elementIndex]
			+ rhs.m[elementIndex];
	}

	return sum;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& rhs) const  {
	Matrix4x4 diff;
	diff.FillWithZeros();

	for (unsigned int elementIndex = 0; elementIndex < 16;
		elementIndex++) {
		diff.m[elementIndex] = this->m[elementIndex]
			- rhs.m[elementIndex];
	}

	return diff;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const {
	Matrix4x4 product;
	product.FillWithZeros();

	for (unsigned int row = 0; row < 4; row++) {
		auto currRow = product[row];

		for (unsigned int column = 0; column < 4;
			column++) {
			for (unsigned int multIndex = 0; multIndex <
				4; multIndex++) {
				currRow[column] += ((*this)(row, multIndex) *
					rhs(multIndex, column));
			}
		}
	}

	return product;
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& rhs) {
	for (unsigned int elementIndex = 0; elementIndex < 16;
		elementIndex++) {
		m[elementIndex] += rhs.m[elementIndex];
	}
	return *this;
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& rhs) {
	for (unsigned int elementIndex = 0; elementIndex < 16;
		elementIndex++) {
		m[elementIndex] -= rhs.m[elementIndex];
	}
	return *this;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rhs) {
	for (unsigned int row = 0; row < 4; row++) {
		auto currRow = (*this)[row];

		for (unsigned int column = 0; column < 4;
			column++) {
			currRow[0] = 0.0f;
			for (unsigned int multIndex = 0; multIndex <
				4; multIndex++) {
				currRow[column] += ((*this)(row, multIndex) *
					rhs(multIndex, column));
			}
		}
	}
	
	return *this;
}

Matrix4x4 Matrix4x4::operator+(float scalar) const {
	Matrix4x4 sum(*this);

	for (unsigned int elementIndex = 0; elementIndex < 16;
		elementIndex++) {
		sum.m[elementIndex] = this->m[elementIndex]
			+ scalar;
	}

	return sum;
}

Matrix4x4 Matrix4x4::operator-(float scalar) const {
	Matrix4x4 diff(*this);

	for (unsigned int elementIndex = 0; elementIndex < 16;
		elementIndex++) {
		diff.m[elementIndex] = this->m[elementIndex]
			- scalar;
	}

	return diff;
}

Matrix4x4 Matrix4x4::operator*(float scalar) const {
	Matrix4x4 product(*this);

	for (unsigned int elementIndex = 0; elementIndex < 16;
		elementIndex++) {
		product.m[elementIndex] = this->m[elementIndex]
			* scalar;
	}

	return product;
}

Matrix4x4 Matrix4x4::operator/(float scalar) const {
	Matrix4x4 div(*this);

	for (unsigned int elementIndex = 0; elementIndex < 16;
		elementIndex++) {
		div.m[elementIndex] = this->m[elementIndex]
			/ scalar;
	}

	return div;
}

Matrix4x4& Matrix4x4::operator+=(float scalar) {
	for (unsigned int elementIndex = 0; elementIndex < 16;
		elementIndex++) {
		m[elementIndex] += scalar;
	}

	return *this;
}

Matrix4x4& Matrix4x4::operator-=(float scalar) {
	for (unsigned int elementIndex = 0; elementIndex < 16;
		elementIndex++) {
		m[elementIndex] -= scalar;
	}

	return *this;
}

Matrix4x4& Matrix4x4::operator*=(float scalar) {
	for (unsigned int elementIndex = 0; elementIndex < 16;
		elementIndex++) {
		m[elementIndex] *= scalar;
	}

	return *this;
}

Matrix4x4& Matrix4x4::operator/=(float scalar) {
	for (unsigned int elementIndex = 0; elementIndex < 16;
		elementIndex++) {
		m[elementIndex] /= scalar;
	}

	return *this;
}

Point3 Matrix4x4::operator*(const Point3& rhs) const {
	return Point3(m[0]*rhs[0] + m[1]*rhs[1] + m[2]*rhs[2] + m[3],
				  m[4]*rhs[0] + m[5]*rhs[1] + m[6]*rhs[2] + m[7],
				  m[8]*rhs[0] + m[9]*rhs[1] + m[10]*rhs[2] + m[11]);
}

Point4 Matrix4x4::operator*(const Point4& rhs) const {
	return Point4(m[0]*rhs[0] + m[1]*rhs[1] + m[2]*rhs[2] + m[3]*rhs[3],
				  m[4]*rhs[0] + m[5]*rhs[1] + m[6]*rhs[2] + m[7]*rhs[3],
				  m[8]*rhs[0] + m[9]*rhs[1] + m[10]*rhs[2] + m[11]*rhs[3]);
}

// assumes vector has w-coordinate of 0 for no translation
Vector3 Matrix4x4::operator*(const Vector3& rhs) const {
	return Vector3(m[0]*rhs[0] + m[1]*rhs[1] + m[2]*rhs[2],
				   m[4]*rhs[0] + m[5]*rhs[1] + m[6]*rhs[2],
				   m[8]*rhs[0] + m[9]*rhs[1] + m[10]*rhs[2]);
}

Matrix4x4 Matrix4x4::Transpose() const {
	Matrix4x4 transposed;

	for (unsigned int row = 0; row < 4; row++) {
		for (unsigned int column = 0; column < 4; column++) {
			transposed[row][column] = (*this)(column, row);
		}
	}
	return transposed;
}

void Matrix4x4::MakeIdentity() {
	for (unsigned int rowIndex = 0, oneDimIndex = 0; rowIndex < 4;
		rowIndex++) {
		for (unsigned int colIndex = 0; colIndex < 4;
			colIndex++, oneDimIndex++) {
			m[oneDimIndex] = (rowIndex == colIndex) ? 1.0f : 0.0f;
		}
	}
}

void Matrix4x4::FillWithZeros() {
	for (unsigned int elementIndex = 0; elementIndex <
		16; elementIndex++) {
		m[elementIndex] = 0.0f;
	}
}

void Matrix4x4::FillWithRandomValues(float min, float max) {
	for (unsigned int elementIndex = 0; elementIndex <
		16; elementIndex++) {
		m[elementIndex] = CommonMath::RandomUnitValue();
	}
}

void Matrix4x4::Print() const {
	for (unsigned int rowIndex = 0, oneDimIndex = 0; rowIndex < 4;
		rowIndex++) {
		for (unsigned int colIndex = 0; colIndex < 4;
			colIndex++, oneDimIndex++) {
			std::cout << m[oneDimIndex] << ", ";
		}
		std::cout << std::endl;
	}
}

Matrix4x4 Matrix4x4::TranslationMatrix(const Vector3& translationVec) {
	return Matrix4x4(
		1.0f, 0.0f, 0.0f, translationVec[0],
		0.0f, 1.0f, 0.0f, translationVec[1],
		0.0f, 0.0f, 1.0f, translationVec[2],
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::InvTranslationMatrix(const Vector3& translationVec) {
	return Matrix4x4(1.0f, 0.0f, 0.0f,-translationVec[0],
					 0.0f, 1.0f, 0.0f,-translationVec[1],
					 0.0f, 0.0f, 1.0f,-translationVec[2],
					 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::ScaleMatrix(const Vector3& scaleVec) {
	return Matrix4x4(
		scaleVec[0], 0.0f, 0.0f, 0.0f,
		0.0f, scaleVec[1], 0.0f, 0.0f,
		0.0f, 0.0f, scaleVec[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::InvScaleMatrix(const Vector3& scaleVec) {
	return Matrix4x4(
		1.0f/scaleVec[0], 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f/scaleVec[1], 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f/scaleVec[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::RotationMatrixX(float angleDegrees) {
	float cosAngle = cos(angleDegrees*DEG_2_RAD);
	float sinAngle = sin(angleDegrees*DEG_2_RAD);
	return Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosAngle, -sinAngle, 0.0f,
		0.0f, sinAngle, cosAngle, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::InvRotationMatrixX(float angleDegrees) {
	float cosAngle = cos(angleDegrees*DEG_2_RAD);
	float sinAngle = sin(angleDegrees*DEG_2_RAD);
	return Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosAngle, sinAngle, 0.0f,
		0.0f,-sinAngle, cosAngle, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::RotationMatrixY(float angleDegrees) {
	float cosAngle = cos(angleDegrees*DEG_2_RAD);
	float sinAngle = sin(angleDegrees*DEG_2_RAD);
	return Matrix4x4(
		cosAngle, 0.0f, sinAngle, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sinAngle, 0.0f, cosAngle, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::InvRotationMatrixY(float angleDegrees) {
	float cosAngle = cos(angleDegrees*DEG_2_RAD);
	float sinAngle = sin(angleDegrees*DEG_2_RAD);
	return Matrix4x4(
		cosAngle, 0.0f,-sinAngle, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinAngle, 0.0f, cosAngle, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::RotationMatrixZ(float angleDegrees) {
	float cosAngle = cos(angleDegrees*DEG_2_RAD);
	float sinAngle = sin(angleDegrees*DEG_2_RAD);
	return Matrix4x4(
		cosAngle, -sinAngle, 0.0f, 0.0f,
		sinAngle, cosAngle, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::InvRotationMatrixZ(float angleDegrees) {
	float cosAngle = cos(angleDegrees*DEG_2_RAD);
	float sinAngle = sin(angleDegrees*DEG_2_RAD);
	return Matrix4x4(
		cosAngle, sinAngle, 0.0f, 0.0f,
		-sinAngle, cosAngle, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

