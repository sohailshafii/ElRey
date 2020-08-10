#pragma once

#include "Math/Matrix.h"

class Matrix4x4 : public Matrix {
public:
	Matrix4x4();
	Matrix4x4(float m00, float m01, float m02, float m03,
			  float m10, float m11, float m12, float m13,
			  float m20, float m21, float m22, float m23,
			  float m30, float m31, float m32, float m33);
	
	Matrix4x4(const Matrix4x4 &rhs);
	Matrix4x4& operator=(const Matrix4x4& rhs);
	
	Matrix4x4 Transpose() const;
	
	static Matrix4x4 TranslationMatrix(const Vector3& translationVec);
	static Matrix4x4 InvTranslationMatrix(const Vector3& translationVec);
	
	static Matrix4x4 ScaleMatrix(const Vector3& scaleVec);
	static Matrix4x4 InvScaleMatrix(const Vector3& scaleVec);
	
	static Matrix4x4 RotationMatrixX(float angleDegrees);
	static Matrix4x4 InvRotationMatrixX(float angleDegrees);
	
	static Matrix4x4 RotationMatrixY(float angleDegrees);
	static Matrix4x4 InvRotationMatrixY(float angleDegrees);
	
	static Matrix4x4 RotationMatrixZ(float angleDegrees);
	static Matrix4x4 InvRotationMatrixZ(float angleDegrees);
};
