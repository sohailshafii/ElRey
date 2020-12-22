#pragma once

#include "ThirdParty/nlohmann/json.hpp"

class Primitive;
class InstancePrimitive;
class Scene;

class PrimitiveLoader {
public:
	static void AddPrimitivesToScene(Scene* scene,
									 nlohmann::json const & objectsArray);
	
	static InstancePrimitive* CreateInstancePrimitive(Scene* scene,
											  const nlohmann::json& jsonObj);
	static Primitive* CreatePrimitive(const nlohmann::json& jsonObj);
	
	static void LoadPly(Scene* scene,
						const nlohmann::json& jsonObj);
};
