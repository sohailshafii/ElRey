#pragma once

#include "Math/Point3.h"
#include "Math/Matrix4x4.h"

class MappingLayer {
public:
	virtual ~MappingLayer() {
	}
	
	virtual void ComputeTextureCoordinates(ShadingInfo const & shadingInfo,
										   int width, int height,
										   int & row, int & column) = 0;
	
	void ApplyTransformations(Point3 & pntTransform) {
		pntTransform = invTransformMatrix * pntTransform;
	}
	
	void ApplyScale(Vector3 const & scale) {
		auto invScaleMatrix = Matrix4x4::InvScaleMatrix(scale);
		invTransformMatrix = invTransformMatrix * invScaleMatrix;
	}
	
	void ApplyTranslation(Vector3 const & translate) {
		auto invTranslate = Matrix4x4::InvTranslationMatrix(translate);
		invTransformMatrix = invTransformMatrix * invTranslate;
	}
	
	void ApplyRotationX(float angle) {
		auto invRot = Matrix4x4::InvRotationMatrixX(angle);
		invTransformMatrix = invTransformMatrix * invRot;
	}
	
	void ApplyRotationY(float angle) {
		auto invRot = Matrix4x4::InvRotationMatrixY(angle);
		invTransformMatrix = invTransformMatrix * invRot;
	}
	
	void ApplyRotationZ(float angle) {
		auto invRot = Matrix4x4::InvRotationMatrixZ(angle);
		invTransformMatrix = invTransformMatrix * invRot;
	}
	
	void ClearTransformations() {
		invTransformMatrix.MakeIdentity();
	}
	
	void SetInvTransformMatrix(Matrix4x4 const & input) {
		invTransformMatrix = input;
	}

protected:
	Matrix4x4 invTransformMatrix;
};

