
#include "PrimitiveLoader.h"
#include "Primitives/InstancePrimitive.h"
#include "Primitives/Sphere.h"
#include "Primitives/Plane.h"
#include "Primitives/Rectangle.h"
#include "Primitives/AABBoxPrim.h"
#include "Primitives/Triangle.h"
#include "Primitives/Torus.h"
#include "Primitives/OpenCylinder.h"
#include "Primitives/Disk.h"
#include "Primitives/CompoundObject.h"
#include "Primitives/TriangleMeshPrim.h"
#include "Primitives/TriangleMesh.h"
#include "SceneData/Scene.h"
#include "SceneData/CommonLoaderFunctions.h"
#include "Math/CommonMath.h"
#include <sstream>

void PrimitiveLoader::AddPrimitivesToScene(Scene* scene,
										   nlohmann::json const & objectsArray) {
	std::vector<nlohmann::json> instancePrimitiveJsonObjs;
	
	for(auto& element : objectsArray.items()) {
		nlohmann::json elementJson = element.value();
		std::string typeName = CommonLoaderFunctions::SafeGetToken(
																   elementJson, "type");
		// create instances after the rest have been assembled,
		// because instance reference normal primitives by name
		if (typeName == "instance") {
			instancePrimitiveJsonObjs.push_back(elementJson);
		}
		else if (typeName == "ply") {
			PrimitiveLoader::LoadPly(scene, elementJson);
		}
		else {
			Primitive* newPrimitive = PrimitiveLoader::CreatePrimitive(elementJson);
			scene->AddPrimitive(newPrimitive);
		}
	}
	
	if (instancePrimitiveJsonObjs.size() > 0) {
		for(auto & instancePrimObj : instancePrimitiveJsonObjs) {
			InstancePrimitive* instancePrim = CreateInstancePrimitive(scene,
																	  instancePrimObj);
			if (instancePrim != nullptr) {
				scene->AddPrimitive(instancePrim);
				// original primitive cannot be used for intersections
				// that's because the instance will be used for intersections instead
				Primitive* originalPrimitive = scene->FindPrimitiveByName(
					instancePrim->GetOriginalPrimName());
				if (originalPrimitive != nullptr) {
					originalPrimitive->SetUsedForInstancing(true);
					std::cout << originalPrimitive->GetName() << " used for instancing.\n";
				}
			}
			else {
				std::stringstream exceptionMsg;
				exceptionMsg << "Could not find original primitive for instance: " <<
					instancePrim->GetName() << ".\n";
				throw exceptionMsg;
			}
		}
	}
}

InstancePrimitive* PrimitiveLoader::CreateInstancePrimitive(Scene* scene,
													const nlohmann::json& jsonObj) {

	InstancePrimitive* newPrimitive = nullptr;
	std::string objectName = CommonLoaderFunctions::SafeGetToken(jsonObj, "name");
	std::string instanceName = CommonLoaderFunctions::SafeGetToken(jsonObj, "inst_name");

	Primitive* originalPrimitive = nullptr;
	unsigned int numPrimitives = scene->GetNumPrimitives();
	for(unsigned int primIndex = 0; primIndex < numPrimitives; primIndex++) {
		Primitive *currPrimitive = scene->FindPrimitiveByName(instanceName);
		if (currPrimitive != nullptr) {
			originalPrimitive = currPrimitive;
			break;
		}
	}
	
	newPrimitive = new InstancePrimitive(objectName, originalPrimitive);
	InstancePrimitive* instancePrim = dynamic_cast<InstancePrimitive*>(newPrimitive);
	if (CommonLoaderFunctions::HasKey(jsonObj, "local_to_world_matrix")) {
		Matrix4x4 localToWorld =
			CommonLoaderFunctions::ConstructMatrixFromJsonNode(jsonObj["local_to_world_matrix"]);
		instancePrim->SetLocalToWorld(localToWorld);
	}
	if (CommonLoaderFunctions::HasKey(jsonObj, "world_to_local_matrix")) {
		Matrix4x4 worldToLocal =
			CommonLoaderFunctions::ConstructMatrixFromJsonNode(jsonObj["world_to_local_matrix"]);
		instancePrim->SetWorldToLocal(worldToLocal);
	}
	if (CommonLoaderFunctions::HasKey(jsonObj, "local_to_world_transform")) {
		Matrix4x4 worldToLocal;
		Matrix4x4 localToWorld;
		CommonLoaderFunctions::SetUpTransformFromJsonNode(
		CommonLoaderFunctions::SafeGetToken(jsonObj, "local_to_world_transform"),
											localToWorld, worldToLocal);
		instancePrim->SetTransformAndInverse(localToWorld, worldToLocal);
	}
	
	return newPrimitive;
}

Primitive* PrimitiveLoader::CreatePrimitive(const nlohmann::json& jsonObj) {
	std::string primitiveType = CommonLoaderFunctions::SafeGetToken(jsonObj, "type");
	Primitive* newPrimitive = nullptr;
	// TODO: Re-factor, getting huge
	if (primitiveType == "plane") {
		auto planeOrigin = CommonLoaderFunctions::SafeGetToken(jsonObj, "position");
		auto planeNormal = CommonLoaderFunctions::SafeGetToken(jsonObj, "normal");
		auto materialNode = CommonLoaderFunctions::SafeGetToken(jsonObj, "material");
		std::string objectName = CommonLoaderFunctions::SafeGetToken(jsonObj, "name");

		std::shared_ptr<Material> objMaterial = CommonLoaderFunctions::CreateMaterial(materialNode);
		newPrimitive = new Plane(
			Point3((float)planeOrigin[0],(float)planeOrigin[1],
				(float)planeOrigin[2]),
			Vector3((float)planeNormal[0],
				(float)planeNormal[1],(float)planeNormal[2]),
			objMaterial, objectName);
	}
	else if (primitiveType == "sphere") {
		auto sphereOrigin = CommonLoaderFunctions::SafeGetToken(jsonObj, "position");
		float sphereRadius = CommonLoaderFunctions::SafeGetToken(jsonObj, "radius");
		auto materialNode = CommonLoaderFunctions::SafeGetToken(jsonObj, "material");
		std::string objectName = CommonLoaderFunctions::SafeGetToken(jsonObj, "name");
		
		std::shared_ptr<Material> objMaterial = CommonLoaderFunctions::CreateMaterial(materialNode);
		newPrimitive = new Sphere(
			Point3((float)sphereOrigin[0],(float)sphereOrigin[1],
					(float)sphereOrigin[2]),
			sphereRadius, objMaterial, objectName);
	}
	else if (primitiveType == "torus") {
		auto materialNode = CommonLoaderFunctions::SafeGetToken(jsonObj, "material");
		std::string objectName = CommonLoaderFunctions::SafeGetToken(jsonObj, "name");
		float sweptRadius = CommonLoaderFunctions::SafeGetToken(jsonObj, "swept_radius");
		float tubeRadius = CommonLoaderFunctions::SafeGetToken(jsonObj, "tube_radius");
		
		std::shared_ptr<Material> objMaterial = CommonLoaderFunctions::CreateMaterial(materialNode);
		newPrimitive = new Torus(sweptRadius, tubeRadius,
								 objMaterial, objectName);
	}
	else if (primitiveType == "aabox") {
		auto minPoint = CommonLoaderFunctions::SafeGetToken(jsonObj, "min_point");
		auto maxPoint = CommonLoaderFunctions::SafeGetToken(jsonObj, "max_point");
		auto materialNode = CommonLoaderFunctions::SafeGetToken(jsonObj, "material");
		std::string objectName = CommonLoaderFunctions::SafeGetToken(jsonObj, "name");
		auto samplerNode = CommonLoaderFunctions::SafeGetToken(jsonObj, "sampler");
		
		std::shared_ptr<Material> objMaterial = CommonLoaderFunctions::CreateMaterial(materialNode);
		RandomSamplerType randomSamplerType;
		int numRandomSamples, numRandomSets;
		CommonLoaderFunctions::SetUpRandomSampler(samplerNode, randomSamplerType, numRandomSamples,
			numRandomSets);
		std::shared_ptr<GenericSampler> newSampler;
		newSampler.reset(SamplerCreator::CreatorSampler(randomSamplerType,
				numRandomSamples, numRandomSets));
		
		newPrimitive = new AABBoxPrim(
			Point3((float)minPoint[0],(float)minPoint[1],
					(float)minPoint[2]),
			Point3((float)maxPoint[0],(float)maxPoint[1],
					(float)maxPoint[2]),
			objMaterial, newSampler, objectName);
	}
	else if (primitiveType == "rectangle") {
		std::string objectName = CommonLoaderFunctions::SafeGetToken(jsonObj, "name");
		auto origin = CommonLoaderFunctions::SafeGetToken(jsonObj, "origin");
		auto sideVec1 = CommonLoaderFunctions::SafeGetToken(jsonObj, "side_vec_1");
		auto sideVec2 = CommonLoaderFunctions::SafeGetToken(jsonObj, "side_vec_2");
		auto materialNode = CommonLoaderFunctions::SafeGetToken(jsonObj, "material");
		auto samplerNode = CommonLoaderFunctions::SafeGetToken(jsonObj, "sampler");

		std::shared_ptr<Material> objMaterial = CommonLoaderFunctions::CreateMaterial(materialNode);

		RandomSamplerType randomSamplerType;
		int numRandomSamples, numRandomSets;
		CommonLoaderFunctions::SetUpRandomSampler(samplerNode,
												  randomSamplerType, numRandomSamples,
												  numRandomSets);
		std::shared_ptr<GenericSampler> newSampler;
		newSampler.reset(SamplerCreator::CreatorSampler(randomSamplerType,
				numRandomSamples, numRandomSets));
		newPrimitive = new Rectangle(Point3((float)origin[0], (float)origin[1],
			(float)origin[2]), Vector3((float)sideVec1[0], (float)sideVec1[1],
			(float)sideVec1[2]), Vector3((float)sideVec2[0], (float)sideVec2[1],
			(float)sideVec2[2]), objMaterial,
			newSampler, objectName);
	}
	else if (primitiveType == "disk") {
		std::string objectName = CommonLoaderFunctions::SafeGetToken(jsonObj, "name");
		auto materialNode = CommonLoaderFunctions::SafeGetToken(jsonObj, "material");
		std::shared_ptr<Material> objMaterial = CommonLoaderFunctions::CreateMaterial(materialNode);
		auto centerPnt = CommonLoaderFunctions::SafeGetToken(jsonObj, "center");
		auto normalVec = CommonLoaderFunctions::SafeGetToken(jsonObj, "normal");
		float radius = CommonLoaderFunctions::SafeGetToken(jsonObj, "radius");
		
		newPrimitive = new Disk(Point3((float)centerPnt[0], (float)centerPnt[1],
										(float)centerPnt[2]),
								Vector3((float)normalVec[0], (float)normalVec[1],
										(float)normalVec[2]), radius, objMaterial,
								objectName);
	}
	else if (primitiveType == "compound_object") {
		std::string objectName = CommonLoaderFunctions::SafeGetToken(jsonObj, "name");
		auto materialNode = CommonLoaderFunctions::SafeGetToken(jsonObj, "material");
		std::shared_ptr<Material> objMaterial = CommonLoaderFunctions::CreateMaterial(materialNode);
		
		CompoundObject* compoundObject = new CompoundObject(objMaterial, objectName);
		
		nlohmann::json childrenArray = jsonObj["children"];
		for(auto & child : childrenArray) {
			Primitive* childPrim = CreatePrimitive(child);
			compoundObject->AddPrimitive(childPrim);
		}
		
		newPrimitive = compoundObject;
	}
	else if (primitiveType == "open_cylinder"){
		std::string objectName = CommonLoaderFunctions::SafeGetToken(jsonObj, "name");
		auto materialNode = CommonLoaderFunctions::SafeGetToken(jsonObj, "material");
		float y0 = CommonLoaderFunctions::SafeGetToken(jsonObj, "y0");
		float y1 = CommonLoaderFunctions::SafeGetToken(jsonObj, "y1");
		float radius = CommonLoaderFunctions::SafeGetToken(jsonObj, "radius");

		std::shared_ptr<Material> objMaterial = CommonLoaderFunctions::CreateMaterial(materialNode);
		
		newPrimitive = new OpenCylinder(y0, y1, radius, objMaterial, objectName);
	}
	else {
		std::stringstream exceptionMsg;
		exceptionMsg << "Could not recognize type: " << primitiveType
			<< ".\n";
		throw exceptionMsg;
	}
	
	return newPrimitive;
}

// borrowed from raytracing from the ground up text
// originally from Greg Turk...pasted commented:
// Most of this function was written by Greg Turk and is released under the licence agreement
// at the start of the PLY.h and PLY.c files
// The PLY.h file is #included at the start of this file
// It still has some of his printf (now cout) statements for debugging
// I've made changes to construct mesh triangles and store them in the grid
// mesh_ptr is a data member of Grid
// objects is a data member of Compound
// triangle_type is either flat or smooth
// Using the one function construct to flat and smooth triangles saves a lot of repeated code
// The ply file is the same for flat and smooth triangles
void PrimitiveLoader::LoadPly(Scene* scene,
							  const nlohmann::json& jsonObj) {
	std::string fileName = CommonLoaderFunctions::SafeGetToken(jsonObj, "file_name");
	bool isSmooth = CommonLoaderFunctions::SafeGetToken(jsonObj, "is_smooth");
	auto materialNode = CommonLoaderFunctions::SafeGetToken(jsonObj, "material");
	std::shared_ptr<Material> objMaterial = CommonLoaderFunctions::CreateMaterial(materialNode);
	std::string objectName = CommonLoaderFunctions::SafeGetToken(jsonObj, "name");
	bool reverseNormals = CommonLoaderFunctions::SafeGetToken(jsonObj, "reverse_normals");
	
	std::cout << "Loading PLY file: " << fileName << "...\n";
	
	const char *xName = "x";
	const char *yName = "y";
	const char *zName = "z";
	PlyProperty vertProps[] = {
		{(char*)xName, PLY_FLOAT, PLY_FLOAT, offsetof(Vertex, x), 0, 0, 0, 0},
		{(char*)yName, PLY_FLOAT, PLY_FLOAT, offsetof(Vertex, y), 0, 0, 0, 0},
		{(char*)zName, PLY_FLOAT, PLY_FLOAT, offsetof(Vertex, x), 0, 0, 0, 0}
	};
	
	const char* vertIndicesName = "vertex_indices";
	PlyProperty faceProps[] = {
		(char*)vertIndicesName, PLY_INT, PLY_INT, offsetof(Face, verts),
		1, PLY_UCHAR, PLY_UCHAR, offsetof(Face, nverts)
	};
	
	PlyFile* ply;
	// number of elements. 2 in our case; vertices and faces
	char** elist;
	int fileType;
	float version;
	PlyProperty** plist;
	
	const char* fileNameStr = fileName.c_str();
	int nelems;
	ply = ply_open_for_reading((char*)fileNameStr, &nelems, &elist, &fileType, &version);
	
	std::cout << "Version " << version << ", type " << fileType << ".\n";
	
	unsigned int numVertices;
	std::shared_ptr<TriangleMesh> triangleMesh = std::make_shared<TriangleMesh>();
	Vertex* vertexPtr = new Vertex;
	Face* facePtr = new Face;
	char const * vertexName = "vertex";
	char const * faceName = "face";
	std::vector<Primitive*> objects;
	
	// there are only two elements in our files: vertices and faces
	for (int elemIndex = 0; elemIndex < nelems; elemIndex++) {
		// get description of first element
		char* elemName = elist[elemIndex];
		int nprops;
		int numElems;
		plist = ply_get_element_description(ply, elemName, &numElems, &nprops);
		
		std::cout << "Element name: " << elemName << ", num elements: " << numElems
			<< ", num properties: " << nprops << std::endl;
		
		if (equal_strings((char *)vertexName, elemName)) {
			SetUpVerts(ply, elemName, vertProps, numElems, vertexPtr,
					   triangleMesh, numVertices);
		}
		
		if (equal_strings((char*)faceName, elemName)) {
			SetUpFace(ply, elemName, faceProps, numElems, isSmooth, facePtr,
					  triangleMesh, objects, objMaterial,
					  objectName, reverseNormals);
		}
		
		for (int propIndex = 0; propIndex < nprops; propIndex++) {
			std::cout << "Property: " << plist[propIndex]->name << "\n";
		}
	}
	
	int numComments;
	char** comments = ply_get_comments(ply, &numComments);
	for (int commentIndex = 0; commentIndex < numComments; commentIndex++) {
		std::cout << "Comment: " << comments[commentIndex] << std::endl;
	}
	
	int numObjInfo;
	char** objInfo = ply_get_obj_info(ply, &numObjInfo);
	for (int infoIndex = 0; infoIndex < numObjInfo; infoIndex++) {
		std::cout << "obj_info = " << objInfo[infoIndex] << std::endl;
	}
	
	ply_close(ply);
		
	delete vertexPtr;
	delete facePtr;
	
	if (isSmooth) {
		std::cout << "Computing smooth normals..n\n";
		ComputeSmoothMeshNormals(triangleMesh, objects);
		std::cout << "Done computing normals!\n";
	}
	
	scene->AddPrimitives(objects);
}

void PrimitiveLoader::SetUpVerts(PlyFile* ply, char* elemName, PlyProperty vertProps[],
								 int numElems, Vertex* vertexPtr,
								 std::shared_ptr<TriangleMesh> triangleMesh,
								 unsigned int numVertices)
{
	// set up for getting vertex elements
	// the three properties are the vertex coords
	ply_get_property(ply, elemName, &vertProps[0]);
	ply_get_property(ply, elemName, &vertProps[1]);
	ply_get_property(ply, elemName, &vertProps[2]);
	
	// reserve mesh elements
	triangleMesh->vertices.clear();
	numVertices = numElems;
	triangleMesh->vertices.reserve(numElems);
	
	for (int j = 0; j < numElems; j++) {
		ply_get_element(ply, (void*)vertexPtr);
		triangleMesh->vertices.push_back(Point3(vertexPtr->x, vertexPtr->y,
								vertexPtr->z));
	}
}

void PrimitiveLoader::SetUpFace(PlyFile* ply, char* elemName, PlyProperty faceProps[],
								int numElems, bool isSmooth, Face* facePtr,
								std::shared_ptr<TriangleMesh> triangleMesh,
								std::vector<Primitive*>& allPrimitives,
								std::shared_ptr<Material> material,
								std::string primName,
								bool reverseNormals) {
	ply_get_property(ply, elemName, &faceProps[0]);
	
	triangleMesh->normals.clear();
	triangleMesh->vertexFaces.clear();
	
	triangleMesh->vertexFaces.reserve(triangleMesh->vertices.size());
	std::vector<unsigned int> faceList;
	
	size_t numVerts = triangleMesh->vertices.size();
	for (size_t vertIndex = 0; vertIndex < numVerts; vertIndex++) {
		triangleMesh->vertexFaces.push_back(faceList);
	}
	
	int numFacesRead = 0;
	for (unsigned int elemIndex = 0; elemIndex < numElems; elemIndex++) {
		ply_get_element(ply, (void *) facePtr);
		TriangleMeshPrimitive* newTriangleMeshPrim =
			new TriangleMeshPrimitive(material, primName, triangleMesh,
									  facePtr->verts[0], facePtr->verts[1],
									  facePtr->verts[2], isSmooth, reverseNormals);
		allPrimitives.push_back(newTriangleMeshPrim);
		
		if (isSmooth) {
			triangleMesh->vertexFaces[facePtr->verts[0]].push_back(numFacesRead);
			triangleMesh->vertexFaces[facePtr->verts[1]].push_back(numFacesRead);
			triangleMesh->vertexFaces[facePtr->verts[2]].push_back(numFacesRead);
			numFacesRead++;
		}
	}
	
	if (!isSmooth) {
		triangleMesh->vertexFaces.erase(triangleMesh->vertexFaces.begin(),
										triangleMesh->vertexFaces.end());
	}
}

void PrimitiveLoader::ComputeSmoothMeshNormals(std::shared_ptr<TriangleMesh> triangleMesh,
											   std::vector<Primitive*>& allPrimitives) {
	size_t numVerts = triangleMesh->vertices.size();
	triangleMesh->normals.reserve(triangleMesh->vertices.size());
	
	for (size_t index = 0; index < numVerts; index++) {
		Vector3 normal;
		Point3 dummyPoint;
		
		std::vector<unsigned int>& vertexFaces = triangleMesh->vertexFaces[index];
		size_t numFaces = vertexFaces.size();
		for (size_t faceIndex = 0; faceIndex < numFaces; faceIndex++) {
			// doesn't matter what vector is given; it's a normal for a face
			normal += allPrimitives[vertexFaces[faceIndex]]->ComputeHardNormal(dummyPoint);
		}
		
		if (fabs(normal[0]) < EPSILON && fabs(normal[1]) < EPSILON &&
			fabs(normal[2]) < EPSILON) {
			normal[1] = 1.0;
		}
		else {
			normal.Normalize();
		}
		
		triangleMesh->normals.push_back(normal);
	}
	
	for (size_t index = 0; index < numVerts; index++) {
		std::vector<unsigned int>& vertexFaces = triangleMesh->vertexFaces[index];
		size_t numFaces = vertexFaces.size();
		for (size_t faceIndex = 0; faceIndex < numFaces; faceIndex++) {
			triangleMesh->vertexFaces[faceIndex].erase(vertexFaces.begin(),
													   vertexFaces.end());
		}
	}
	
	triangleMesh->vertexFaces.erase(triangleMesh->vertexFaces.begin(),
									triangleMesh->vertexFaces.end());
}
