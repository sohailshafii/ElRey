#pragma once

#include "ThirdParty/nlohmann/json.hpp"
#include "Point3.h"
#include <vector>

class Primitive;
class InstancePrimitive;
class Scene;
class TriangleMesh;
class TriangleMeshPrimitive;
class Material;

// from https://vulkan-tutorial.com/Loading_models
struct Vertex {
	float pos[3];
	float texCoord[2];

	bool operator==(const Vertex& other) const {
		return fabs(pos[0] - other.pos[0]) < 0.001f &&
			fabs(pos[1] - other.pos[1]) < 0.001f &&
			fabs(pos[2] - other.pos[2]) < 0.001f &&
			fabs(texCoord[0] - other.texCoord[0]) < 0.001f &&
			fabs(texCoord[1] - other.texCoord[1]) < 0.001f;
	}
};

namespace std {
  template<> struct hash<Vertex> {
	size_t operator()(Vertex const& v) const {
		// oh boy, hope this is correct!
		return ((hash<float>()(v.pos[0]) ^
				 hash<float>()(v.pos[1]) ^
				 hash<float>()(v.pos[2]) ^
				 hash<float>()(v.texCoord[0]) ^
				 hash<float>()(v.texCoord[1])) << 1) >> 1;
	}
  };
}

class PrimitiveLoader {
private:
	struct ModelPrimitiveInfo {
		std::vector<Primitive*> primitives;
		std::string name;
	};
public:
	static void AddPrimitivesToScene(Scene* scene,
									 nlohmann::json const & objectsArray);
	
	static InstancePrimitive* CreateInstancePrimitive(Scene* scene,
											  const nlohmann::json& jsonObj);
	static Primitive* CreatePrimitive(const nlohmann::json& jsonObj);
	
	// OBJ only for now
	static void LoadModel(ModelPrimitiveInfo* primInfo,
						  const nlohmann::json& jsonObj);
	
private:
	static void AddFaceIndex(TriangleMesh *mesh,
							 unsigned int vertIndex,
							 unsigned int triIndex);
	static void ComputeSmoothMeshNormals(std::shared_ptr<TriangleMesh>
										 triangleMesh,
										 std::vector<Primitive*>& allPrimitives);
};
