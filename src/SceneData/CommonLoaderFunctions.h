#pragma once

#include "ThirdParty/nlohmann/json.hpp"
#include "Sampling/SamplerCreator.h"
#include "Math/Matrix4x4.h"
#include <string>

class Material;
class AbstractTexture;

class CommonLoaderFunctions {
public:
	static nlohmann::json SafeGetToken(nlohmann::json const & jsonObj,
									   std::string const & key);
	static bool HasKey(nlohmann::json const & jsonObj,
					   std::string const & key);
	
	static std::shared_ptr<Material> CreateMaterial(nlohmann::json const & jsonObj);
	
	static std::shared_ptr<AbstractTexture> CreateTexture(nlohmann::json const &
														  jsonObject,
														  std::string const & colorKey);
	
	static void SetUpRandomSampler(nlohmann::json const & jsonObj,
								   RandomSamplerType & randomSamplerType,
								   int & numRandomSamples, int & numRandomSets);
	
	static Matrix4x4 ConstructMatrixFromJsonNode(nlohmann::json const & matrixNode);
	
	static void SetUpTransformFromJsonNode(nlohmann::json const & transformNode,
										   Matrix4x4 & localToWorld,
										   Matrix4x4 & worldToLocal);
};
