#include "SceneData/CommonLoaderFunctions.h"
#include "Materials/LambertianMaterial.h"
#include "Materials/PhongMaterial.h"
#include "Materials/SimpleEmissiveMaterial.h"
#include <sstream>

nlohmann::json CommonLoaderFunctions::SafeGetToken(const nlohmann::json& jsonObj,
										 const std::string& key) {
	if (jsonObj.find(key) != jsonObj.end()) {
		return jsonObj[key];
	}
	std::stringstream exceptionMsg;
	exceptionMsg << "Could not find key: " << key
		<< " in JSON object: " << jsonObj << ".\n";
	throw exceptionMsg;
}

std::shared_ptr<Material> CommonLoaderFunctions::CreateMaterial(const nlohmann::json& jsonObj) {
	std::shared_ptr<Material> newMaterial;

	// TODO: should BRDFs have four-component colors?
	// think about transparency
	std::string primitiveType = SafeGetToken(jsonObj, "type");
	if (primitiveType == "lambertian") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		auto colorObj = SafeGetToken(jsonObj, "color");
		newMaterial = std::make_shared<LambertianMaterial>(kA, kD,
			Color3(colorObj[0], colorObj[1],
				colorObj[2]));
	}
	else if (primitiveType == "phong") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		float kS = SafeGetToken(jsonObj, "ks");
		float exponent = SafeGetToken(jsonObj, "exponent");
		auto colorObj = SafeGetToken(jsonObj, "color");
		auto ksColor = SafeGetToken(jsonObj, "ks_color");
		newMaterial = std::make_shared<PhongMaterial>(kA, kD, kS, exponent,
			Color3(colorObj[0], colorObj[1],
				colorObj[2]),
			Color3(ksColor[0], ksColor[1],
				ksColor[2]));
	}
	else if (primitiveType == "simple_emissive") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		auto colorObj = SafeGetToken(jsonObj, "color");
		newMaterial = std::make_shared<SimpleEmissiveMaterial>
			(kA, kD, Color3(colorObj[0], colorObj[1], colorObj[2]));
	}
	return newMaterial;
}
