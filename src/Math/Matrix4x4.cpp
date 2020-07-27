
#include "Matrix4x4.h"

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
