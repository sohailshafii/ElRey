#pragma once

#include "Math/Point3.h"
#include "Math/Vector3.h"

class Camera {
public:
	Camera();
	Camera(const Point3& eyePosition, const Point3& lookAtPosition,
		float horizontalFov, const Vector3& up);

	virtual ~Camera() { }

	// TODO: fix based on actual camera being used
	virtual Vector3 GetRayDirectionToWorldPoint(const Point3& point)
		const = 0;

protected:
	void ComputeCoordinateFrameAxes();

private:
	Point3 eyePosition;
	Point3 lookAtPosition;
	float horizontalFov;

	Vector3 right;
	Vector3 up;
	Vector3 forward;
};
