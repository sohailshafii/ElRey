#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "Materials/Texturing/AbstractTexture.h"

class InstanceTextureRegistry {
public:
	InstanceTextureRegistry() { }
	// enforce as a singleton
	InstanceTextureRegistry(InstanceTextureRegistry const &) = delete;
	InstanceTextureRegistry& operator=
	(InstanceTextureRegistry const &) = delete;
	
	static InstanceTextureRegistry& GetInstance();
	
	std::shared_ptr<AbstractTexture> GetTextureForName(std::string const & name);
	
	void AddTexture(std::shared_ptr<AbstractTexture> const & texture);
	
private:
	std::unordered_map<std::string, std::shared_ptr<AbstractTexture>>
		nameToTexture;
};

