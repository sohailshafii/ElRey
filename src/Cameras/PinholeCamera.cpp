
#include "PinholeCamera.h"

PinholeCamera::PinholeCamera(const Point3& eyePosition, const Point3& lookAtPosition,
	float horizontalFov, const Vector3& up, float viewPlaneDistance)
	: Camera(eyePosition, lookAtPosition, horizontalFov, up) {
	this->viewPlaneDistance = viewPlaneDistance;
}

Vector3 PinholeCamera::GetRayDirectionToWorldPoint(const Point3& point) const {
	return Vector3::Forward(); // TODO fix
}
