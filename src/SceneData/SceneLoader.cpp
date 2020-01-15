#include "SceneLoader.h"
#include "ThirdParty/nlohmann/json.hpp"
#include <fstream>
#include <exception>
#include <iostream>
#include <sstream>
#include "SceneData/Scene.h"
#include "Math/Sphere.h"
#include "Math/Plane.h"
#include "SceneData/Light.h"
#include "SceneData/AmbientLight.h"
#include "SceneData/DirectionalLight.h"
#include "SceneData/PointLight.h"

static Primitive* CreatePrimitive(const nlohmann::json& jsonObj);
static Light* CreateLight(const nlohmann::json& jsonObj);

void SceneLoader::DeserializeJSONFileIntoScene(class Scene* scene,
											   const std::string &jsonFilePath) {
	try {
		std::ifstream jsonFile(jsonFilePath);
		nlohmann::json jsonObj;
	
		jsonFile >> jsonObj;
		
		nlohmann::json objectsArray = jsonObj["objects"];
		for(auto& element : objectsArray.items()) {
			Primitive* newPrimitive = CreatePrimitive(element.value());
			if (newPrimitive != nullptr) {
				scene->AddPrimitive(newPrimitive);
			}
		}

		nlohmann::json lightsArray = jsonObj["lights"];
		for (auto& element : lightsArray.items()) {
			Light* newLight = CreateLight(element.value());
			if (newLight != nullptr) {
				scene->AddLight(newLight);
			}
		}
	}
	catch(const std::exception& e) {
		std::cout << "Could not deserialize JSON file: " << jsonFilePath
			<< ". Reason: " << e.what() << std::endl;
	}
}

static inline nlohmann::json SafeGetToken(const nlohmann::json& jsonObj,
									const std::string& key) {
	if (jsonObj.find(key) != jsonObj.end()) {
		return jsonObj[key];
	}
	std::stringstream exceptionMsg;
	exceptionMsg << "Could not find key: " << key
		<< " in JSON object: " << jsonObj << ".\n";
	throw exceptionMsg;
}

static Primitive* CreatePrimitive(const nlohmann::json& jsonObj) {
	std::string primitiveType = SafeGetToken(jsonObj, "type");
	Primitive* newPrimitive = nullptr;
	if (primitiveType == "plane") {
		auto planeOrigin = SafeGetToken(jsonObj, "position");
		auto planeNormal = SafeGetToken(jsonObj, "normal");
		auto planeColor = SafeGetToken(jsonObj, "color");
		newPrimitive = new Plane(
			Point3((float)planeOrigin[0],(float)planeOrigin[1],
				(float)planeOrigin[2]),
			Vector3((float)planeNormal[0],
				(float)planeNormal[1],(float)planeNormal[2]),
			Color((float)planeColor[0], (float)planeColor[1],
				(float)planeColor[2], (float)planeColor[3]));
	}
	else if (primitiveType == "sphere") {
		auto sphereOrigin = SafeGetToken(jsonObj, "position");
		float sphereRadius = SafeGetToken(jsonObj, "radius");
		auto sphereColor = SafeGetToken(jsonObj, "color");
		newPrimitive = new Sphere(
			Point3((float)sphereOrigin[0],(float)sphereOrigin[1],
					(float)sphereOrigin[2]),
			sphereRadius,
			Color((float)sphereColor[0], (float)sphereColor[1],
					(float)sphereColor[2], (float)sphereColor[3]));
	}
	else {
		std::stringstream exceptionMsg;
		exceptionMsg << "Could not recognize type: " << primitiveType
			<< ".\n";
		throw exceptionMsg;
	}
	return newPrimitive;
}

Light* CreateLight(const nlohmann::json& jsonObj) {
	std::string lightType = SafeGetToken(jsonObj, "type");
	Light* newLight = nullptr;
	std::string primitiveType = SafeGetToken(jsonObj, "type");
	if (primitiveType == "ambient") {
		auto radiance = SafeGetToken(jsonObj, "radiance");
		float radianceScale = SafeGetToken(jsonObj, "radianceScale");
		newLight = new AmbientLight(Color3((float)radiance[0], (float)radiance[1],
			(float)radiance[2]), radianceScale);
	}
	else if (primitiveType == "directional") {
		auto direction = SafeGetToken(jsonObj, "direction");
		auto radiance = SafeGetToken(jsonObj, "radiance");
		float radianceScale = SafeGetToken(jsonObj, "radianceScale");
		newLight = new DirectionalLight(Vector3((float)direction[0], (float)direction[1],
			(float)direction[2]), Color3((float)radiance[0], (float)radiance[1],
			(float)radiance[2]), radianceScale);
	}
	else if (primitiveType == "point") {
		auto position = SafeGetToken(jsonObj, "position");
		auto radiance = SafeGetToken(jsonObj, "radiance");
		float radianceScale = SafeGetToken(jsonObj, "radianceScale");
		newLight = new PointLight(Point3((float)position[0], (float)position[1],
			(float)position[2]), Color3((float)radiance[0], (float)radiance[1],
			(float)radiance[2]), radianceScale);
	}
	return newLight;
}
