#include "Camera.h"
#include "CommonMath.h"
#include <cmath>

#include "Sampling/RandomSampler.h"
#include "Sampling/OneSampleSampler.h"
#include "Sampling/JitteredSampler.h"
#include "Sampling/NRooksSampler.h"
#include "Sampling/MultiJitteredSampler.h"

#include "Math/Point2.h"
#include "Math/Ray.h"
#include "SceneData/Scene.h"

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
	this->up.Normalize();
	ComputeForward();
	ComputeCoordinateRightAndUp();
	this->numColumnsPixels = numColumnsPixels;
	this->numRowsPixels = numRowsPixels;
	this->numPixels = numColumnsPixels*numRowsPixels;
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
	
	unsigned int numPixels = numColumnsPixels*numRowsPixels;
	gridPositions = new Point2[numPixels];
	pixelRowHeight = viewPlaneHeight / numRowsPixels;
	pixelColWidth = viewPlaneWidth / numColumnsPixels;
	
	for (unsigned int row = 0, pixel = 0; row < numRowsPixels; row++) {
		for (unsigned int column = 0; column < numColumnsPixels; column++, pixel++) {
			gridPositions[pixel] = Point2(
				pixelColWidth * (float)column - viewPlaneWidth * 0.5f,
				-pixelRowHeight * (float)row + viewPlaneHeight * 0.5f);
		}
	}
}

void Camera::ComputeForward() {
	forward = lookAtPosition - eyePosition;
	forward.Normalize();
}

// Note that up is recomputed to be perpendicular
// to both right and forward. This is known as Gram-Schmidt
// orthonormalization. This coordinate system is left-handed.
void Camera::ComputeCoordinateRightAndUp() {
	// turn on sanity check in case we need it but leave it off to
	// avoid excess computational cost
#if 1
	// edge-case: what if up and forward to are parallel or
	// anti-parallel?
	auto dotProduct = forward * up;
	if (fabs(fabs(dotProduct) - 1.0f) < EPSILON) {
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

void Camera::Displace(const Vector3& displacementVector) {
	// don't use overloaded operators += for speed
	eyePosition[0] += displacementVector[0];
	eyePosition[1] += displacementVector[1];
	eyePosition[2] += displacementVector[2];
}

void Camera::Rotate(const Matrix& matrix) {
	right = matrix*right;
	up = matrix*up;
	forward = matrix*forward;
}

void Camera::Transform(const Matrix& matrix) {
	right = matrix*right;
	up = matrix*up;
	forward = matrix*forward;
	eyePosition = matrix*eyePosition;
}

void Camera::TranslateAndRotate(const Vector3& translation, float rightRotationDegrees,
	float upRotationDegrees) {
	float yawRad = upRotationDegrees*DEG_2_RAD;
	float pitchRad = rightRotationDegrees*DEG_2_RAD;
	float cosPitchRad = cos(pitchRad);
	forward[0] = cos(yawRad) * cosPitchRad;
    forward[1] = sin(pitchRad);
    forward[2] = sin(yawRad) * cosPitchRad;
	forward.Normalize();
	right = Vector3::Up() ^ forward;
	right.Normalize();
	up = forward ^ right;
	up.Normalize();
	
	eyePosition += right*translation[0] + forward*translation[2];
	this->lookAtPosition = eyePosition + forward*viewPlaneDistance;
}

void Camera::CastIntoScene(unsigned char* pixels, unsigned int bytesPerPixel,
						   const Scene* scene, float frameTime) const {
	unsigned int numSamples = viewPlaneSampler->GetNumSamples();
	Ray rayToCast;
	rayToCast.SetOrigin(eyePosition);
	float invGamma = GetInvGamma();
	float maxCastDistance = std::numeric_limits<float>::max();
	float finalColorMultFactor = GetFinalPixelMultFact();

	for (unsigned int pixelIndex = 0, byteIndex = 0; pixelIndex < numPixels;
		pixelIndex++, byteIndex += bytesPerPixel) {
		float tMax = std::numeric_limits<float>::max();
		Color accumColor = Color::Black();
		Color sampleColor = Color::Black();
		const Point2& oldOrigin = gridPositions[pixelIndex];
		for (unsigned int sampleIndex = 0; sampleIndex < numSamples; sampleIndex++) {
			sampleColor = Color::Black();
			Point2 newSample = viewPlaneSampler->GetSampleOnUnitSquare();
			Point2 newPixelPnt(oldOrigin[0] + pixelColWidth * newSample[0],
							   oldOrigin[1] + pixelRowHeight * newSample[1]);
			AffectFirstRay(rayToCast, newPixelPnt);
			
			tMax = maxCastDistance;
			scene->Intersect(rayToCast, sampleColor, 0.0f, tMax);
			accumColor += sampleColor;
		}

		accumColor *= finalColorMultFactor;
		// gamma-correct
		accumColor ^= invGamma;
		// RGB->BGR (related to SDL)
		pixels[byteIndex] = (unsigned char)(accumColor[2] * 255.0f); // B
		pixels[byteIndex + 1] = (unsigned char)(accumColor[1] * 255.0f); // G
		pixels[byteIndex + 2] = (unsigned char)(accumColor[0] * 255.0f); // R
		if (bytesPerPixel == 4) {
			pixels[byteIndex + 3] = 255;
		}
	}
}
