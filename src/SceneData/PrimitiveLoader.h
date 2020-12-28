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

class PrimitiveLoader {
public:
	static void AddPrimitivesToScene(Scene* scene,
									 nlohmann::json const & objectsArray);
	
	static InstancePrimitive* CreateInstancePrimitive(Scene* scene,
											  const nlohmann::json& jsonObj);
	static Primitive* CreatePrimitive(const nlohmann::json& jsonObj);
	
	// OBJ only for now
	static void LoadModel(Scene* scene,
						  const nlohmann::json& jsonObj);
	
private:
	/*typedef struct Vertex {
		float x, y, z;
	} Vertex;
	
	typedef struct Face {
		unsigned char nverts;
		int* verts;
	} Face;
	
	static void SetUpVerts(PlyFile* ply, char* elemName, PlyProperty vertProps[],
						   int numElems, Vertex* vertexPtr,
						   std::shared_ptr<TriangleMesh> triangleMesh,
						   unsigned int numVertices);
	
	static void SetUpFace(PlyFile* ply, char* elemName, PlyProperty faceProps[],
						  int numElems, bool isSmooth, Face* facePtr,
						  std::shared_ptr<TriangleMesh> triangleMesh,
						  std::vector<Primitive*>& allPrimitives,
						  std::shared_ptr<Material> material,
						  std::string primName,
						  bool reverseNormals);
	
	static void ComputeSmoothMeshNormals(std::shared_ptr<TriangleMesh> triangleMesh,
										 std::vector<Primitive*>& allPrimitives);*/
};
