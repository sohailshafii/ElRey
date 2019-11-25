#include "Camera.h"

Camera::Camera() {
	//this->eyePosition = 
}

Camera::Camera(Point4 eyePosition, Point4 lookAtPosition,
	float horizontalFov, Vector3 up) {
	this->eyePosition = eyePosition;
	this->lookAtPosition = lookAtPosition;
	this->horizontalFov = horizontalFov;
	this->eyePosition = eyePosition;
}
