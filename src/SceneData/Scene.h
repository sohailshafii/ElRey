#pragma once

#include "Primitives/Primitive.h"
#include "AccelerationStructures/BaseAccelerator.h"
#include "SceneData/Light.h"
#include "Cameras/Camera.h"
#include <string>
#include <vector>

// simple base class for all acceleration structures, or really anything
// that represents our world.
class Scene {
public:
	Scene(BaseAccelerator::AcceleratorType acceleratorType);
	Scene(Primitive **primitives, unsigned int numPrimitives,
		  BaseAccelerator::AcceleratorType acceleratorType);
	virtual ~Scene();

	void AddPrimitive(Primitive *newPrimitive) {
		return baseAccelerator->AddPrimitive(newPrimitive);
	}
	
	void AddPrimitives(Primitive **newPrimitives,
					   unsigned int numNewPrimitives) {
		return baseAccelerator->AddPrimitives(newPrimitives,
											  numNewPrimitives);
	}
	
	void RemovePrimitive(Primitive* primitiveToRemove) {
		return baseAccelerator->RemovePrimitive(primitiveToRemove);
	}

	virtual void AddLight(Light* newLight);
	virtual void AddLights(Light** newLights, unsigned int numNewLights);
	
	virtual void SetAmbientLight(Light* newAmbientLight);

	virtual bool Intersect(const Ray &ray, Color &newColor,
		float tMin, float& tMax) const;

	Primitive* GetPrimitive(unsigned int index) {
		return baseAccelerator->GetPrimitive(index);
	}

	Primitive* FindPrimitiveByName(const std::string& name) {
		return baseAccelerator->FindPrimitiveByName(name);
	}

	unsigned int GetNumPrimitives() const {
		return baseAccelerator->GetNumPrimitives();
	}
	
	unsigned int GetNumLights() const {
		return lights.size();
	}
	
	Light* GetLight(unsigned int index) {
		return lights[index];
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
	void CastIntoScene(unsigned char* pixels, unsigned int bytesPerPixel, float frameTime, bool BGRMode);
	
	unsigned int GetNumRowsPixels() const {
		return mainCamera->GetNumRowsPixels();
	}
	
	unsigned int GetNumColumnsPixels() const {
		return mainCamera->GetNumColumnsPixels();
	}
	
	// exclude a primitive if we are checking area light and want
	// to exclude their related primitive
	bool ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
										float tMax,
										const Primitive* primitiveToExclude = nullptr) const;

private:
	void CleanUpLights();

protected:
	BaseAccelerator *baseAccelerator;
	std::vector<Light*> lights;
	Light* ambientLight;
	
	Camera* mainCamera;
	bool allowNavigation;
};
