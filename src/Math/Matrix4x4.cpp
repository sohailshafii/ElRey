
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

Matrix4x4 Matrix4x4::RotationMatrix(const Vector3& axis, float angleDegrees) {
	float cosAngle = cos(angleDegrees*DEG_2_RAD);
	float sinAngle = sin(angleDegrees*DEG_2_RAD);
	float oneMinusCos = 1.0f - cosAngle;
	float uxUy = axis[0] * axis[1];
	float uzUy = axis[2] * axis[1];
	float uzUx = axis[2] * axis[0];
	return Matrix4x4(
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

Matrix4x4 Matrix4x4::InvRotationMatrix(const Vector3& axis, float angleDegrees) {
	float cosAngle = cos(-angleDegrees*DEG_2_RAD);
	float sinAngle = sin(-angleDegrees*DEG_2_RAD);
	float oneMinusCos = 1.0f - cosAngle;
	float uxUy = axis[0] * axis[1];
	float uzUy = axis[2] * axis[1];
	float uzUx = axis[2] * axis[0];
	return Matrix4x4(
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
	float cosAngle = cos(-angleDegrees*DEG_2_RAD);
	float sinAngle = sin(-angleDegrees*DEG_2_RAD);
	return Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosAngle, -sinAngle, 0.0f,
		0.0f, sinAngle, cosAngle, 0.0f,
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
	float cosAngle = cos(-angleDegrees*DEG_2_RAD);
	float sinAngle = sin(-angleDegrees*DEG_2_RAD);
	return Matrix4x4(
		cosAngle, 0.0f, sinAngle, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sinAngle, 0.0f, cosAngle, 0.0f,
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
	float cosAngle = cos(-angleDegrees*DEG_2_RAD);
	float sinAngle = sin(-angleDegrees*DEG_2_RAD);
	return Matrix4x4(
		cosAngle, -sinAngle, 0.0f, 0.0f,
		sinAngle, cosAngle, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

