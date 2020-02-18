#pragma once

#include "Math/Primitive.h"
#include "SceneData/Light.h"
#include "Cameras/Camera.h"

// simple base class for all acceleration structures, or really anything
// that represents our world.
class Scene {
public:
	Scene();
	Scene(Primitive **primitives, unsigned int numPrimitives);
	virtual ~Scene();

	virtual void AddPrimitive(Primitive *newPrimitive);
	virtual void AddPrimitives(Primitive **newPrimitives,
		unsigned int numNewPrimitives);

	virtual void AddLight(Light* newLight);
	virtual void AddLights(Light** newLights,
		unsigned int numNewLights);

	virtual bool Intersect(const Ray &ray, Color &newColor,
		float tMin, float& tMax) const;

	Primitive* GetPrimitive(unsigned int index) {
		return primitives[index];	
	}

	unsigned int GetNumPrimitives() const {
		return numPrimitives;
	}
	
	void SetCamera(Camera *newCamera) {
		if (mainCamera != nullptr) {
			delete mainCamera;
		}
		mainCamera = newCamera;
	}
	
	void SetAllowNavigation(bool allowNavigation) {
		this->allowNavigation = allowNavigation;
	}
	
	void TranslateAndRotateCamera();
	void CastIntoScene();
	
	void TranslateAndRotate(const Vector3& translation, float rightRotationDegrees,
							float upRotationDegrees);
	void CastIntoScene(unsigned char* pixels, unsigned int bytesPerPixel, float frameTime);
	
	unsigned int GetNumRowsPixels() const {
		return mainCamera->GetNumRowsPixels();
	}
	
	unsigned int GetNumColumnsPixels() const {
		return mainCamera->GetNumColumnsPixels();
	}

private:
	void CleanUpPrimitives(Primitive **primitivesToClean, unsigned int
		numToClean);
	void CleanUpLights(Light** lightsToClean, unsigned int numToClean);
	
	bool TestShadowFeeler(IntersectionResult& interesectionResult);

protected:
	// each item is a pointer to a primitive type,
	// whatever that might be
	Primitive** primitives;
	unsigned int numPrimitives;

	Light** lights;
	unsigned int numLights;
	
	Camera* mainCamera;
	bool allowNavigation;
};
