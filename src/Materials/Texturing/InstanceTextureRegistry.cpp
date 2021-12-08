#include "InstanceTextureRegistry.h"

InstanceTextureRegistry& InstanceTextureRegistry::GetInstance() {
	static InstanceTextureRegistry instance;
	return instance;
}

std::shared_ptr<AbstractTexture> InstanceTextureRegistry::GetTextureForName(std::string const & name) {
	std::shared_ptr<AbstractTexture> texture = nullptr;
	if (nameToTexture.find(name) != nameToTexture.end()) {
		texture = nameToTexture[name];
	}
	
	return texture;
}

void InstanceTextureRegistry::AddTexture(std::shared_ptr<AbstractTexture> const & texture) {
	nameToTexture[texture->GetName()] = texture;
}
