#include "SceneData/CommonLoaderFunctions.h"
#include "Materials/LambertianMaterial.h"
#include "Materials/PhongMaterial.h"
#include "Materials/SimpleEmissiveMaterial.h"
#include <sstream>

nlohmann::json CommonLoaderFunctions::SafeGetToken(const nlohmann::json& jsonObj,
										 const std::string& key) {
	if (HasKey(jsonObj, key)) {
		return jsonObj[key];
	}
	std::stringstream exceptionMsg;
	exceptionMsg << "Could not find key: " << key
		<< " in JSON object: " << jsonObj << ".\n";
	throw exceptionMsg;
}

bool CommonLoaderFunctions::HasKey(const nlohmann::json& jsonObj,
			const std::string& key) {
	return (jsonObj.find(key) != jsonObj.end());
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

void CommonLoaderFunctions::SetUpRandomSampler(const nlohmann::json& jsonObj,
						RandomSamplerType& randomSamplerType,
						int &numRandomSamples, int &numRandomSets) {
	randomSamplerType = None;
	std::string samplerTypeToken = CommonLoaderFunctions::SafeGetToken(jsonObj, "random_sampler_type");
	if (samplerTypeToken == "random") {
		randomSamplerType = Random;
	}
	else if (samplerTypeToken == "regular") {
		randomSamplerType = Regular;
	}
	else if (samplerTypeToken == "jittered") {
		randomSamplerType = Jittered;
	}
	else if (samplerTypeToken == "nrooks") {
		randomSamplerType = NRooks;
	}
	else if (samplerTypeToken == "multijittered") {
		randomSamplerType = MultiJittered;
	}
	else if (samplerTypeToken == "none") {
		// es ok
	}
	else {
		std::stringstream exceptionMsg;
		exceptionMsg << "Cannot understand sampler type specified: " <<
			samplerTypeToken.c_str() << ".\n";
		throw exceptionMsg;
	}
	numRandomSamples = CommonLoaderFunctions::SafeGetToken(jsonObj, "num_random_samples");
	numRandomSets = CommonLoaderFunctions::SafeGetToken(jsonObj, "num_random_sets");
}


Matrix4x4 CommonLoaderFunctions::ConstructMatrixFromJsonNode(
															 const nlohmann::json& matrixNode) {
	auto matrixRow0 = CommonLoaderFunctions::SafeGetToken(matrixNode,
														  "matrix_row_0");
	auto matrixRow1 = CommonLoaderFunctions::SafeGetToken(matrixNode,
														  "matrix_row_1");
	auto matrixRow2 = CommonLoaderFunctions::SafeGetToken(matrixNode,
														  "matrix_row_2");
	auto matrixRow3 = CommonLoaderFunctions::SafeGetToken(matrixNode,
														  "matrix_row_3");
	
	return Matrix4x4(
	(float)matrixRow0[0], (float)matrixRow0[1], (float)matrixRow0[2], (float)matrixRow0[3],
	(float)matrixRow1[0], (float)matrixRow1[1], (float)matrixRow1[2], (float)matrixRow1[3],
	(float)matrixRow2[0], (float)matrixRow2[1], (float)matrixRow2[2], (float)matrixRow2[3],
	(float)matrixRow3[0], (float)matrixRow3[1], (float)matrixRow3[2], (float)matrixRow3[3]);
}
