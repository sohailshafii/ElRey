#pragma once

#include <string>

class SceneLoader {
public:
	static void DeserializeJSONFileIntoScene(class Scene* scene,
										const std::string &jsonFilePath);
};
