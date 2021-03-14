#pragma once

#include "Primitives/Primitive.h"
#include "ThirdParty/nlohmann/json.hpp"
#include "SceneData/Light.h"
#include "WorldData/SimpleWorld.h"
#include "Cameras/Camera.h"
#include <string>
#include <vector>

// simple base class for all acceleration structures, or really anything
// that represents our world.
class Scene {
public:
	Scene();
	Scene(std::shared_ptr<Primitive> *primitives, unsigned int numPrimitives);
	virtual ~Scene();

	void AddPrimitive(std::shared_ptr<Primitive> newPrimitive) {
		return simpleWorld->AddPrimitive(newPrimitive);
	}
	
	void AddPrimitives(std::shared_ptr<Primitive> *newPrimitives,
					   unsigned int numNewPrimitives) {
		return simpleWorld->AddPrimitives(newPrimitives,
											  numNewPrimitives);
	}
	
	void AddPrimitives(std::vector<std::shared_ptr<Primitive>> newPrimitives) {
		return simpleWorld->AddPrimitives(newPrimitives);
	}
	
	void RemovePrimitive(std::shared_ptr<Primitive> primitiveToRemove) {
		simpleWorld->RemovePrimitive(primitiveToRemove);
	}
	
	void RemovePrimitiveByName(std::string const & name) {
		simpleWorld->RemovePrimitiveByName(name);
	}

	virtual void AddLight(Light* newLight);
	virtual void AddLights(Light** newLights, unsigned int numNewLights);
	
	virtual void SetAmbientLight(Light* newAmbientLight);

	virtual bool Intersect(const Ray &ray, Color &newColor,
		float tMin, float& tMax, int bounceCount) const;

	std::shared_ptr<Primitive> GetPrimitive(unsigned int index) {
		return simpleWorld->GetPrimitive(index);
	}

	std::shared_ptr<Primitive> FindPrimitiveByName(const std::string& name) {
		return simpleWorld->FindPrimitiveByName(name);
	}

	unsigned int GetNumPrimitives() const {
		return simpleWorld->GetNumPrimitives();
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
	
	void GetMaxBounceCount() const {
		return maxBounceCount;
	}
	
	void GetMaxBounceCount(int maxBounceCount) {
		this->maxBounceCount = maxBounceCount;
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
										float tMax) const;

private:
	void CleanUpLights();

protected:
	SimpleWorld *simpleWorld;
	std::vector<Light*> lights;
	Light* ambientLight;
	
	Camera* mainCamera;
	bool allowNavigation;
	int maxBounceCount;
};
