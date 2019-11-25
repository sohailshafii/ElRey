#pragma once

#include "Math/Vector3.h"
#include "Math/Point4.h"

class Camera {
public:
	Camera();
	Camera(Point4 eyePosition, Point4 lookAtPosition,
		float horizontalFov, Vector3 up);

private:
	Point4 eyePosition;
	Point4 lookAtPosition;
	float horizontalFov;
	Vector3 up;

	Vector3 forward;
	Vector3 right;
};
