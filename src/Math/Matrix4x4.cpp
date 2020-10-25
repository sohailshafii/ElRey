
#include "Matrix4x4.h"
#include "CommonMath.h"

Matrix4x4::Matrix4x4() : Matrix(4, 4) {
}

Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03,
					 float m10, float m11, float m12, float m13,
					 float m20, float m21, float m22, float m23,
					 float m30, float m31, float m32, float m33)
	: Matrix(m00, m01, m02, m03,
			 m10, m11, m12, m13,
			 m20, m21, m22, m23,
			 m30, m31, m32, m33) {
}

Matrix4x4::Matrix4x4(Matrix4x4 const & rhs) : Matrix(4, 4) {
	AllocateAndCopyFrom(rhs);
}

Matrix4x4& Matrix4x4::operator=(Matrix4x4 const & rhs) {
	if (&rhs != this) {
		AllocateAndCopyFrom(rhs);
	}

	return *this;
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

