#include "Camera.h"
#include "CommonMath.h"
#include <cmath>

#include "Sampling/RandomSampler.h"
#include "Sampling/OneSampleSampler.h"
#include "Sampling/JitteredSampler.h"
#include "Sampling/NRooksSampler.h"
#include "Sampling/MultiJitteredSampler.h"

Camera::Camera() {
	this->eyePosition = Point3::Zero();
	this->lookAtPosition = Point3::Zero();
	this->right = Vector3::Zero();
	this->up = Vector3::Zero();
	this->forward = Vector3::Zero();
	this->viewPlaneSampler = nullptr;
	this->gridPositions = nullptr;
}

Camera::~Camera() {
	if (viewPlaneSampler != nullptr) {
		delete viewPlaneSampler;
	}
	if (gridPositions != nullptr) {
		delete gridPositions;
	}
}

Camera::Camera(const Point3& eyePosition, const Point3& lookAtPosition,
			   unsigned int numColumnsPixels, unsigned int numRowsPixels, float
			   viewPlaneWidth, float viewPlaneHeight, const Vector3& up, RandomSamplerType
			   randomSamplerType, unsigned int numRandomSamples, unsigned int numRandomSets) {
	this->eyePosition = eyePosition;
	this->lookAtPosition = lookAtPosition;
	this->up = up;
	ComputeCoordinateFrameAxes();
	this->numColumnsPixels = numColumnsPixels;
	this->numRowsPixels = numRowsPixels;
	this->viewPlaneWidth = viewPlaneWidth;
	this->viewPlaneHeight = viewPlaneHeight;
	viewPlaneDistance = (this->lookAtPosition - this->eyePosition).Norm();
	
	switch (randomSamplerType) {
	case Jittered:
		viewPlaneSampler = new JitteredSampler(numRandomSets, numRandomSamples);
		break;
	case Random:
		viewPlaneSampler = new RandomSampler(numRandomSets, numRandomSamples);
		break;
	case NRooks:
		viewPlaneSampler = new NRooksSampler(numRandomSets, numRandomSamples);
		break;
	case MultiJittered:
		viewPlaneSampler = new MultiJitteredSampler(numRandomSets, numRandomSamples);
		break;
	default:
		viewPlaneSampler = new OneSampleSampler();
		break;
	}
	
	maxCastDist = std::numeric_limits<float>::max();
	unsigned int numPixels = numColumnsPixels*numRowsPixels;
	gridPositions = new Point3[numPixels];

	pixelRowHeight = viewPlaneHeight / numRowsPixels;
	pixelColWidth = viewPlaneWidth / numColumnsPixels;
	// center position of upper-left pixel. Note that raster y values
	// increase downwards
	Point3 planeUpperLeft = lookAtPosition +
		right*-(viewPlaneWidth*0.5f + pixelColWidth*0.5f) +
		up*(viewPlaneHeight*0.5f + pixelRowHeight*0.5f);

	for (int row = 0, pixel = 0; row < numRowsPixels; row++) {
		for (int column = 0; column < numColumnsPixels; column++, pixel++) {
			// find pixel center in world space
			Point3 pixelCenterWorld = planeUpperLeft + Point3(
				pixelColWidth*(float)column, -pixelRowHeight*(float)row, 0.0f);
			gridPositions[pixel] = pixelCenterWorld;
		}
	}
	
}

// Note that up is recomputed to be perpendicular
// to both right and forward. This is known as Gram-Schmidt
// orthonormalization.
void Camera::ComputeCoordinateFrameAxes() {
	up.Normalize();
	forward = eyePosition - lookAtPosition;
	forward.Normalize();

	// turn on sanity check in case we need it but leave it off to
	// avoid excess computational cost
#if TEST_CAMERA_COORDINATE_SYSTEM
	// edge-case: what if up and forward to are parallel or
	// anti-parallel?
	auto dotProduct = forward * up;
	if (fabs(dotProduct) - 1.0f < EPSILON) {
		// find unit vector in coordinate axis that is perpendicular
		// to forward
		Vector3 candidateUp = Vector3::Forward();
		Vector3 candidateUp2 = Vector3::Up();
		Vector3 candidateUp3 = Vector3::Right();
		float dot1 = fabs(candidateUp * forward);
		float dot2 = fabs(candidateUp2 * forward);
		float dot3 = fabs(candidateUp3 * forward);
		if (dot1 < dot2 && dot1 < dot3) {
			// subtract out the parallel part
			up = candidateUp - forward*(candidateUp * forward);
		}
		else if (dot2 < dot1 && dot2 < dot3) {
			// subtract out the parallel part
			up = candidateUp2 - forward * (candidateUp2 * forward);
		}
		else {
			// subtract out the parallel part
			up = candidateUp3 - forward * (candidateUp3 * forward);
		}
		up.Normalize();
	}
#endif

	right = up ^ forward;
	right.Normalize();
	up = forward ^ right;
	// (no need to normalize as two normal vectors
	// crossed results in a normal vector)
}
