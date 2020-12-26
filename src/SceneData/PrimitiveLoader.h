#pragma once

#include "ThirdParty/nlohmann/json.hpp"
#include "ThirdParty/ply/ply.h"
#include "Point3.h"
#include <vector>

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
	
private:
	typedef struct Vertex {
		float x, y, z;
	} Vertex;
	
	typedef struct Face {
		unsigned char nverts;
		int* verts;
	} Face;
	
	static void SetUpVerts(PlyFile* ply, char* elemName, PlyProperty vertProps[],
						   int numElems, Vertex* vertexPtr, std::vector<Point3>& vertices, unsigned int numVertices);
};
