#pragma once

#include "Math/Matrix.h"

class Matrix4x4 : public Matrix {
public:
	Matrix4x4();
	Matrix4x4(float m00, float m01, float m02, float m03,
			  float m10, float m11, float m12, float m13,
			  float m20, float m21, float m22, float m23,
			  float m30, float m31, float m32, float m33);
	
	Matrix4x4 TranslationMatrix(const Vector3& translationVec);
	Matrix4x4 InvTranslationMatrix(const Vector3& translationVec);
};