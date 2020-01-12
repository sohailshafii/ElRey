#include "SceneLoader.h"
#include "ThirdParty/nlohmann/json.hpp"
#include <fstream>
#include <exception>
#include <iostream>
#include <sstream>
#include "SceneData/Scene.h"
#include "Math/Sphere.h"
#include "Math/Plane.h"

static Primitive* CreatePrimitive(const nlohmann::json& jsonObj);

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
	}
	catch(const std::exception& e) {
		std::cout << "Could not deserialize JSON file: " << jsonFilePath
			<< ". Reason: " << e.what() << std::endl;
	}
}

static inline nlohmann::json safeGetToken(const nlohmann::json& jsonObj,
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
	std::string primitiveType = safeGetToken(jsonObj, "type");
	Primitive* newPrimitive = nullptr;
	if (primitiveType == "plane") {
		auto planeOrigin = safeGetToken(jsonObj, "position");
		auto planeNormal = safeGetToken(jsonObj, "normal");
		auto planeColor = safeGetToken(jsonObj, "color");
		newPrimitive = new Plane(
			Point3((float)planeOrigin[0],(float)planeOrigin[1],
				(float)planeOrigin[2]),
			Vector3((float)planeNormal[0],
				(float)planeNormal[1],(float)planeNormal[2]),
			Color((float)planeColor[0], (float)planeColor[1],
				(float)planeColor[2], (float)planeColor[3]));
	}
	else if (primitiveType == "sphere") {
		auto sphereOrigin = safeGetToken(jsonObj, "position");
		float sphereRadius = safeGetToken(jsonObj, "radius");
		auto sphereColor = safeGetToken(jsonObj, "color");
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
