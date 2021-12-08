#pragma once

#include "ThirdParty/nlohmann/json.hpp"
#include "Point3.h"
#include "Matrix4x4.h"
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
		std::vector<std::shared_ptr<Primitive>> primitives;
		std::string name;
	};
public:
	static void CreateGridOfGrids(Scene *scene, const nlohmann::json& jsonObj);
	static void CreateGridOfGrids(Scene* scene,
								  int numLevels,
								  int gridRes,
								  float gapPercentage,
								  float bunnySize,
								  Vector3 const & origin);
	
	static void AddPrimitivesToScene(Scene* scene,
									 nlohmann::json const & topmostJsonNode,
									 nlohmann::json const & objectsArray);
	static void AddInstancePrimitives(Scene* scene,
									  std::vector<nlohmann::json> const & instancePrimitiveJsonObjs);
	static void AddGridPrimitives(Scene* scene,
								  std::vector<nlohmann::json> const & gridPrimitives,
								  std::vector<ModelPrimitiveInfo*> & modelPrimitiveInfos);
	
	static std::shared_ptr<InstancePrimitive> CreateInstancePrimitive(std::string const & objectName,
													  std::shared_ptr<Primitive> originalPrimitive,
													  Matrix4x4 const & localToWorld,
													  Matrix4x4 const & worldToLocal);
	
	static std::shared_ptr<InstancePrimitive> CreateInstancePrimitive(Scene* scene,
											  const nlohmann::json& jsonObj);
	static std::shared_ptr<Primitive> CreatePrimitive(nlohmann::json const & topmostJsonNode,
													  const nlohmann::json& jsonObj);
	
	static void LoadModelFromJSON(ModelPrimitiveInfo* primInfo,
								  nlohmann::json const & topmostJsonNode,
								  const nlohmann::json& jsonObj);
	// OBJ only for now
	static void LoadModel(ModelPrimitiveInfo* primInfo,
						  std::string const & fileName,
						  bool isSmooth,
						  std::shared_ptr<Material> objMaterial,
						  std::string const & objectName,
						  bool reverseNormals,
						  Matrix4x4 const & localToWorld);
	
private:
	static void AddFaceIndex(TriangleMesh *mesh,
							 unsigned int vertIndex,
							 unsigned int triIndex);
	static void ComputeSmoothMeshNormals(TriangleMesh* triangleMesh,
										 std::vector<std::shared_ptr<Primitive>>& allPrimitives);
};
