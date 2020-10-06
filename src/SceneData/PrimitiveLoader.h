#pragma once

#include "ThirdParty/nlohmann/json.hpp"

class Primitive;
class InstancePrimitive;

class PrimitiveLoader {
public:
	static void AddPrimitivesToScene(class Scene* scene,
									 nlohmann::json const & objectsArray);
	
	static InstancePrimitive* CreateInstancePrimitive(Scene* scene,
											  const nlohmann::json& jsonObj);
	static Primitive* CreatePrimitive(const nlohmann::json& jsonObj);
};
