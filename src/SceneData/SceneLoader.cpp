#include "SceneLoader.h"
#include "ThirdParty/nlohmann/json.hpp"
#include <fstream>

void SceneLoader::DeserializeJSONFileIntoScene(class Scene* scene,
											   const std::string &jsonFilePath)
{
	std::ifstream jsonFile(jsonFilePath);
	nlohmann::json jsonObj;
	
	jsonFile >> jsonObj;
}
