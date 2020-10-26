#pragma once

#include "Math/Point3.h"
#include "Math/Point4.h"
#include "Math/Vector3.h"

class Matrix4x4 {
public:
	Matrix4x4();
	Matrix4x4(float m00, float m01, float m02, float m03,
			  float m10, float m11, float m12, float m13,
			  float m20, float m21, float m22, float m23,
			  float m30, float m31, float m32, float m33);
	Matrix4x4(const Matrix4x4 &rhs);
	Matrix4x4(Matrix4x4 &&rhs);
	
	~Matrix4x4();
	
	Matrix4x4& operator=(const Matrix4x4& rhs);
	
	Matrix4x4 operator*(const Matrix4x4& rhs) const;
	Matrix4x4 operator+(const Matrix4x4& rhs) const;
	Matrix4x4 operator-(const Matrix4x4& rhs) const;
	
	Matrix4x4& operator+=(const Matrix4x4& rhs);
	Matrix4x4& operator-=(const Matrix4x4& rhs);
	Matrix4x4& operator*=(const Matrix4x4& rhs);
	
	Matrix4x4 operator+(float scalar) const;
	Matrix4x4 operator-(float scalar) const;
	Matrix4x4 operator*(float scalar) const;
	Matrix4x4 operator/(float scalar) const;

	Matrix4x4& operator+=(float scalar);
	Matrix4x4& operator-=(float scalar);
	Matrix4x4& operator*=(float scalar);
	Matrix4x4& operator/=(float scalar);
	
	inline float* GetRow(unsigned int rowIndex);
	inline float* operator[](unsigned int rowIndex);

	inline float& operator()(unsigned int row, unsigned int col);
	inline float operator()(unsigned int row, unsigned int col) const;
	
	Point3 operator*(const Point3& rhs) const;
	Point4 operator*(const Point4& rhs) const;
	Vector3 operator*(const Vector3& rhs) const;
	
	Matrix4x4 Transpose() const;
	void MakeIdentity();
	void FillWithZeros();
	void FillWithRandomValues(float min, float max);
	void Print() const;
	
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
	
private:
	float *m;
	void AllocateAndCopyFrom(const Matrix4x4& other);
};

inline float* Matrix4x4::GetRow(unsigned int rowIndex) {
	return m + rowIndex*4;
}

inline float* Matrix4x4::operator[](unsigned int rowIndex) {
	return m + rowIndex*4;
}

inline float& Matrix4x4::operator()(unsigned int row,
	unsigned int col) {
	return m[row*4 + col];
}

inline float Matrix4x4::operator()(unsigned int row,
	unsigned int col) const {
	return m[row*4 + col];
}
