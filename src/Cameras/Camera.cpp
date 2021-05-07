#include "Camera.h"
#include "CommonMath.h"
#include <cmath>

#include "Math/Point2.h"
#include "Math/Ray.h"
#include "SceneData/Scene.h"
#include "Math/Matrix4x4.h"

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
	CommonMath::ComputeUVWFromUpandForward(this->right, this->up, this->forward);
	this->numColumnsPixels = numColumnsPixels;
	this->numRowsPixels = numRowsPixels;
	this->numPixels = numColumnsPixels*numRowsPixels;
	this->viewPlaneWidth = viewPlaneWidth;
	this->viewPlaneHeight = viewPlaneHeight;
	viewPlaneDistance = (this->lookAtPosition - this->eyePosition).Norm();
	
	viewPlaneSampler = SamplerCreator::CreatorSampler(randomSamplerType,
													  numRandomSamples,
													  numRandomSets);
	
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

void Camera::Displace(const Vector3& displacementVector) {
	// don't use overloaded operators += for speed
	eyePosition[0] += displacementVector[0];
	eyePosition[1] += displacementVector[1];
	eyePosition[2] += displacementVector[2];
}

void Camera::Rotate(const Matrix4x4& matrix) {
	right = matrix*right;
	up = matrix*up;
	forward = matrix*forward;
}

void Camera::Transform(const Matrix4x4& matrix) {
	right = matrix*right;
	up = matrix*up;
	forward = matrix*forward;
	eyePosition = matrix*eyePosition;
}

void Camera::TranslateAndRotate(const Vector3& translation, float rightRotationDegrees,
	float upRotationDegrees) {
	float yawRad = -upRotationDegrees*DEG_2_RAD;
	float pitchRad = rightRotationDegrees*DEG_2_RAD;
	float cosPitchRad = cos(pitchRad);
	forward[0] = sin(yawRad) * cosPitchRad;
	forward[1] = sin(pitchRad);
	forward[2] = cos(yawRad) * cosPitchRad;
	forward.Normalize();
	right = Vector3::Up() ^ forward;
	right.Normalize();
	up = forward ^ right;
	up.Normalize();
	
	eyePosition += right*translation[0] + forward*translation[2];
	this->lookAtPosition = eyePosition + forward*viewPlaneDistance;
}

void Camera::CastIntoScene(unsigned char* pixels, unsigned int bytesPerPixel,
						   const Scene* scene, float frameTime, bool BGRMode) const {
	unsigned int numSamples = viewPlaneSampler->GetNumSamples();
	Ray rayToCast;
	rayToCast.SetOrigin(eyePosition);
	float invGamma = GetInvGamma();
	float maxCastDistance = std::numeric_limits<float>::max();
	float finalColorMultFactor = GetFinalPixelMultFact();
	
	float (Scene::* const traceFunc) (const Ray &, Color3 &,
									 float, float, int) const =
		usePathtracing ? &Scene::PathRaytrace : &Scene::WhittedRaytrace;

	for (unsigned int pixelIndex = 0, byteIndex = 0; pixelIndex < numPixels;
		pixelIndex++, byteIndex += bytesPerPixel) {
		Color3 accumColor = Color3::Black();
		Color3 sampleColor = Color3::Black();
		const Point2& oldOrigin = gridPositions[pixelIndex];
		for (unsigned int sampleIndex = 0; sampleIndex < numSamples; sampleIndex++) {
			sampleColor = Color3::Black();
			Point2 newSample = viewPlaneSampler->GetSampleOnUnitSquare();
			Point2 newPixelPnt(oldOrigin[0] + pixelColWidth * newSample[0],
							   oldOrigin[1] + pixelRowHeight * newSample[1]);
			AffectFirstRay(rayToCast, newPixelPnt);
			
			(scene->*traceFunc)(rayToCast, sampleColor, 0.0f, maxCastDistance, 0);
			accumColor += sampleColor;
		}

		accumColor *= finalColorMultFactor;
		// gamma-correct
		accumColor ^= invGamma;
		// cap colors
		accumColor[0] = ClampColor(accumColor[0]);
		accumColor[1] = ClampColor(accumColor[1]);
		accumColor[2] = ClampColor(accumColor[2]);
		
		// RGB->BGR (related to SDL)
		pixels[byteIndex] = (unsigned char)(accumColor[BGRMode? 2 : 0] * 255.0f); // B
		pixels[byteIndex + 1] = (unsigned char)(accumColor[1] * 255.0f); // G
		pixels[byteIndex + 2] = (unsigned char)(accumColor[BGRMode ? 0 : 2] * 255.0f); // R
		if (bytesPerPixel == 4) {
			pixels[byteIndex + 3] = 255;
		}
	}
}
