#pragma once

#include <string>

class SceneLoader {
public:
	static class Scene* DeserializeJSONFileIntoScene(const std::string &jsonFilePath);
};
