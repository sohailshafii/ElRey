#pragma once

#include "ThirdParty/nlohmann/json.hpp"
#include "Sampling/SamplerCreator.h"
#include "Math/Matrix4x4.h"
#include <string>

class Material;

class CommonLoaderFunctions {
public:
	static nlohmann::json SafeGetToken(const nlohmann::json& jsonObj,
									   const std::string& key);
	static bool HasKey(const nlohmann::json& jsonObj,
					   const std::string& key);
	
	static std::shared_ptr<Material> CreateMaterial(const nlohmann::json& jsonObj);
	
	static void SetUpRandomSampler(const nlohmann::json& jsonObj,
								   RandomSamplerType& randomSamplerType,
								   int &numRandomSamples, int &numRandomSets);
	
	static Matrix4x4 ConstructMatrixFromJsonNode(const nlohmann::json& matrixNode);
};
