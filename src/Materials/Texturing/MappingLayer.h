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
		auto invRot = Matrix4x4::RotationMatrixX(angle);
		invTransformMatrix = invTransformMatrix * invRot;
	}
	
	void ApplyRotationY(float angle) {
		auto invRot = Matrix4x4::RotationMatrixY(angle);
		invTransformMatrix = invTransformMatrix * invRot;
	}
	
	void ApplyRotationZ(float angle) {
		auto invRot = Matrix4x4::RotationMatrixZ(angle);
		invTransformMatrix = invTransformMatrix * invRot;
	}
	
	void ClearTransformations() {
		invTransformMatrix.MakeIdentity();
	}

private:
	Matrix4x4 invTransformMatrix;
};

