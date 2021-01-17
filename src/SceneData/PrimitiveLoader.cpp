
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
#include "Primitives/GridPrimitive.h"
#include "SceneData/Scene.h"
#include "SceneData/CommonLoaderFunctions.h"
#include "Math/CommonMath.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <sstream>
#include <unordered_map>

void PrimitiveLoader::AddPrimitivesToScene(Scene* scene,
										   nlohmann::json const & objectsArray) {
	std::vector<nlohmann::json> instancePrimitiveJsonObjs;
	std::vector<nlohmann::json> gridPrimitives;
	std::vector<ModelPrimitiveInfo*> modelPrimitives;
	
	for(auto& element : objectsArray.items()) {
		nlohmann::json elementJson = element.value();
		std::string typeName = CommonLoaderFunctions::SafeGetToken(elementJson, "type");
		// create instances after the rest have been assembled,
		// because instance reference normal primitives by name
		if (typeName == "instance") {
			instancePrimitiveJsonObjs.push_back(elementJson);
		}
		else if (typeName == "grid") {
			gridPrimitives.push_back(elementJson);
		}
		else if (typeName == "obj_model") {
			ModelPrimitiveInfo *primInfo = new ModelPrimitiveInfo();
			PrimitiveLoader::LoadModel(primInfo, elementJson);
			modelPrimitives.push_back(primInfo);
		}
		else {
			Primitive* newPrimitive = PrimitiveLoader::CreatePrimitive(elementJson);
			scene->AddPrimitive(newPrimitive);
		}
	}
	
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
				std::cout << originalPrimitive->GetName() << " used for instancing.\n";
			}
			else {
				std::stringstream exceptionMsg;
				exceptionMsg << "Could not remove primitive from scene after instancing: " <<
					instancePrim->GetOriginalPrimName() << ".\n";
				throw exceptionMsg;
			}
			scene->RemovePrimitive(originalPrimitive);
		}
		else {
			std::stringstream exceptionMsg;
			exceptionMsg << "Could not find original primitive for instance: " <<
				instancePrim->GetName() << ".\n";
			throw exceptionMsg;
		}
	}
	
	// load in any grids and add children to them from scene
	for(auto & gridPrimJson : gridPrimitives) {
		std::string objectName = CommonLoaderFunctions::SafeGetToken(gridPrimJson, "name");
		float multipier = 1.0f;
		if (CommonLoaderFunctions::HasKey(gridPrimJson, "multipier")) {
			multipier = CommonLoaderFunctions::SafeGetToken(gridPrimJson, "multipier");
		}
		nlohmann::json childrenArray = gridPrimJson["children"];
		std::vector<Primitive*> primitives;
		for(auto & child : childrenArray) {
			std::string primName = child;
			Primitive* originalPrimitive = scene->FindPrimitiveByName(primName);
			if (originalPrimitive != nullptr) {
				std::cout << originalPrimitive->GetName() << " used for grid.\n";
				
				scene->RemovePrimitive(originalPrimitive);
				primitives.push_back(originalPrimitive);
			}
			else {
				std::cerr << "Could not remove primitive due to grid: " <<
					primName << ". Trying models...\n";
				int foundModelIndex = -1;
				for (size_t i = 0; i < modelPrimitives.size(); i++) {
					auto modelPrim = modelPrimitives[i];
					if (modelPrim->name == primName) {
						foundModelIndex = i;
						break;
					}
				}
				// add to grid, and erase from normal models list
				// remainder goes into normal scene
				if (foundModelIndex != -1) {
					auto modelPrim = modelPrimitives[foundModelIndex];
					auto objectsToAdd = modelPrim->primitives;
					for(auto object : objectsToAdd) {
						primitives.push_back(object);
					}
					modelPrimitives.erase(modelPrimitives.begin() + foundModelIndex);
				}
				else {
					std::cerr << "Could not add model " << primName << " to grid.\n";
				}
			}
		}
		GridPrimitive* newPrim = new GridPrimitive(objectName, primitives,
												   multipier);
		scene->AddPrimitive(newPrim);
	}
	
	// add leftover models to main scene, if not part of any accelerators
	for (size_t i = 0; i < modelPrimitives.size(); i++) {
		auto modelPrim = modelPrimitives[i];
		auto modelPrimitives = modelPrim->primitives;
		for(auto object : modelPrimitives) {
			scene->AddPrimitive(object);
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

void PrimitiveLoader::LoadModel(ModelPrimitiveInfo* primInfo,
							  const nlohmann::json& jsonObj) {
	std::string fileName = CommonLoaderFunctions::SafeGetToken(jsonObj, "file_name");
	bool isSmooth = CommonLoaderFunctions::SafeGetToken(jsonObj, "is_smooth");
	auto materialNode = CommonLoaderFunctions::SafeGetToken(jsonObj, "material");
	std::shared_ptr<Material> objMaterial = CommonLoaderFunctions::CreateMaterial(materialNode);
	std::string objectName = CommonLoaderFunctions::SafeGetToken(jsonObj, "name");
	bool reverseNormals = CommonLoaderFunctions::SafeGetToken(jsonObj, "reverse_normals");
	primInfo->name = objectName;
	
	Matrix4x4 localToWorld;
	if (CommonLoaderFunctions::HasKey(jsonObj, "local_to_world_matrix")) {
		localToWorld =
			CommonLoaderFunctions::ConstructMatrixFromJsonNode(jsonObj["local_to_world_matrix"]);
	}
	if (CommonLoaderFunctions::HasKey(jsonObj, "local_to_world_transform")) {
		Matrix4x4 worldToLocal;
		CommonLoaderFunctions::SetUpTransformFromJsonNode(
		CommonLoaderFunctions::SafeGetToken(jsonObj, "local_to_world_transform"),
											localToWorld, worldToLocal);
	}
	
#if __APPLE__
	std::string scenePath = "../../" + fileName;
#else
	std::string scenePath = "../" + fileName;
#endif
	
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	std::cout << "Loading obj file: " << scenePath << "...\n";
	
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
						  scenePath.c_str())) {
		throw std::runtime_error(warn + err);
	}
	
	std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	std::shared_ptr<TriangleMesh> triangleMesh = std::make_shared<TriangleMesh>();
	size_t triangleIndices[3];
	int numTrianglesTotal = 0;
	
	for (const auto& shape : shapes) {
		size_t triIndex = 0;
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};

			vertex.pos[0] = attrib.vertices[3 * index.vertex_index + 0];
			vertex.pos[1] = attrib.vertices[3 * index.vertex_index + 1];
			vertex.pos[2] = attrib.vertices[3 * index.vertex_index + 2];

			if (attrib.texcoords.size() > 0) {
				vertex.texCoord[0] = attrib.texcoords[2 * index.texcoord_index + 0];
				vertex.texCoord[1] = 1.0f - attrib.texcoords[2 * index.texcoord_index + 1];
			}

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(triangleMesh->vertices.size());
				Point3 newPoint(vertex.pos[0], vertex.pos[1], vertex.pos[2]);
				triangleMesh->vertices.push_back(localToWorld * newPoint);
			}
			triangleIndices[triIndex++] = uniqueVertices[vertex];
			
			if (triIndex == 3) {
				size_t primitiveIndex = primInfo->primitives.size();
				std::ostringstream triangleMeshName;
				triangleMeshName << objectName << "-" << primitiveIndex;
				TriangleMeshPrimitive* newTriangleMeshPrim =
					new TriangleMeshPrimitive(objMaterial,
											  triangleMeshName.str(),
											  triangleMesh,
											  triangleIndices[0],
											  triangleIndices[1],
											  triangleIndices[2],
											  isSmooth, reverseNormals);
				primInfo->primitives.push_back(newTriangleMeshPrim);
				numTrianglesTotal++;
				if (isSmooth) {
					size_t faceIndex = primInfo->primitives.size()-1;
					AddFaceIndex(triangleMesh.get(), triangleIndices[0],
								 faceIndex);
					AddFaceIndex(triangleMesh.get(), triangleIndices[1],
								 faceIndex);
					AddFaceIndex(triangleMesh.get(), triangleIndices[2],
								 faceIndex);
				}
				triIndex = 0;
			}
		}
	}
	
	if (!isSmooth) {
		triangleMesh->vertexFaces.erase(triangleMesh->vertexFaces.begin(),
										triangleMesh->vertexFaces.end());
	}
	else {
		std::cout << "Computing smooth normals...\n";
		ComputeSmoothMeshNormals(triangleMesh, primInfo->primitives);
		std::cout << "Done computing normals!\n";
	}
	std::cout << "Generated " << numTrianglesTotal << " for "
		<< fileName << ".\n";
}

void PrimitiveLoader::AddFaceIndex(TriangleMesh *mesh,
								   unsigned int vertIndex,
								   unsigned int triIndex) {
	auto vertFacesSize = mesh->vertexFaces.size();
	if (vertFacesSize == 0 || vertFacesSize < vertIndex+1) {
		std::vector<unsigned int> faceList;
		auto startIndex = (vertFacesSize == 0) ? 0 : mesh->vertexFaces.size()-1;
		for (size_t i = startIndex; i <= vertIndex; i++) {
			mesh->vertexFaces.push_back(faceList);
		}
	}
	mesh->vertexFaces[vertIndex].push_back(triIndex);
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
			auto begin = triangleMesh->vertexFaces[faceIndex].begin();
			auto end = triangleMesh->vertexFaces[faceIndex].end();
			triangleMesh->vertexFaces[faceIndex].erase(begin, end);
		}
	}
	
	triangleMesh->vertexFaces.erase(triangleMesh->vertexFaces.begin(),
									triangleMesh->vertexFaces.end());
}
