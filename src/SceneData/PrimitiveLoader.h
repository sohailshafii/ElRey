#pragma once

#include "ThirdParty/nlohmann/json.hpp"

class Primitive;

class PrimitiveLoader {
public:
	static Primitive* CreatePrimitive(const nlohmann::json& jsonObj);
};
