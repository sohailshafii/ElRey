
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
#include "SceneData/Scene.h"
#include "SceneData/CommonLoaderFunctions.h"
#include <sstream>

void PrimitiveLoader::AddPrimitivesToScene(Scene* scene,
										   nlohmann::json const & objectsArray) {
	std::vector<nlohmann::json> instancePrimitiveJsonObjs;
	
	for(auto& element : objectsArray.items()) {
		nlohmann::json elementJson = element.value();
		Primitive* newPrimitive = PrimitiveLoader::CreatePrimitive(elementJson);
		if (newPrimitive != nullptr) {
			// assemble instances after the rest have been assembled
			if (CommonLoaderFunctions::HasKey(elementJson, "inst_name")) {
				instancePrimitiveJsonObjs.push_back(elementJson);
			}
			else {
				scene->AddPrimitive(newPrimitive);
			}
		}
	}
	
	if (instancePrimitiveJsonObjs.size() > 0) {
		for(auto & instancePrimObj : instancePrimitiveJsonObjs) {
			CreateInstancePrimitive(scene, instancePrimObj);
		}
	}
}

Primitive* PrimitiveLoader::CreateInstancePrimitive(Scene* scene,
													const nlohmann::json& jsonObj) {

	Primitive* newPrimitive = nullptr;
	std::string objectName = CommonLoaderFunctions::SafeGetToken(jsonObj, "name");
	std::string instanceName = CommonLoaderFunctions::SafeGetToken(jsonObj, "inst_name");
	//newPrimitive = new InstancePrimitive()
	Primitive* originalPrimitive = nullptr;
	unsigned int numPrimitives = scene->GetNumPrimitives();
	for(unsigned int primIndex = 0; primIndex < numPrimitives;
		primIndex++) {
		Primitive *currPrimitive = scene->GetPrimitive(primIndex);
		if (currPrimitive->GetName() == instanceName) {
			originalPrimitive = currPrimitive;
			break;
		}
	}
	
	newPrimitive = new InstancePrimitive(objectName, originalPrimitive);
	if (CommonLoaderFunctions::HasKey(jsonObj, "local_to_world_matrix")) {
		Matrix4x4 localToWorld =
			CommonLoaderFunctions::ConstructMatrixFromJsonNode(jsonObj["local_to_world_matrix"]);
		newPrimitive->SetLocalToWorld(localToWorld);
	}
	if (CommonLoaderFunctions::HasKey(jsonObj, "world_to_local_matrix")) {
		Matrix4x4 worldToLocal =
			CommonLoaderFunctions::ConstructMatrixFromJsonNode(jsonObj["world_to_local_matrix"]);
		newPrimitive->SetWorldToLocal(worldToLocal);
	}
	if (CommonLoaderFunctions::HasKey(jsonObj, "local_to_world_transform")) {
		Matrix4x4 worldToLocal;
		Matrix4x4 localToWorld;
		CommonLoaderFunctions::SetUpTransformFromJsonNode(
		CommonLoaderFunctions::SafeGetToken(jsonObj, "local_to_world_transform"),
														  worldToLocal, localToWorld);
		newPrimitive->SetTransformAndInverse(worldToLocal, localToWorld);
	}
	
	return newPrimitive;
}

Primitive* PrimitiveLoader::CreatePrimitive(const nlohmann::json& jsonObj) {
	std::string primitiveType = CommonLoaderFunctions::SafeGetToken(jsonObj, "type");
	Primitive* newPrimitive = nullptr;
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
	
	// TODO: remove once primitive base class doesn't have transform information
	if (newPrimitive != nullptr) {
		if (CommonLoaderFunctions::HasKey(jsonObj, "local_to_world_matrix")) {
			Matrix4x4 localToWorld =
				CommonLoaderFunctions::ConstructMatrixFromJsonNode(jsonObj["local_to_world_matrix"]);
			newPrimitive->SetLocalToWorld(localToWorld);
		}
		if (CommonLoaderFunctions::HasKey(jsonObj, "world_to_local_matrix")) {
			Matrix4x4 worldToLocal =
				CommonLoaderFunctions::ConstructMatrixFromJsonNode(jsonObj["world_to_local_matrix"]);
			newPrimitive->SetWorldToLocal(worldToLocal);
		}
		if (CommonLoaderFunctions::HasKey(jsonObj, "local_to_world_transform")) {
			Matrix4x4 worldToLocal;
			Matrix4x4 localToWorld;
			CommonLoaderFunctions::SetUpTransformFromJsonNode(
			CommonLoaderFunctions::SafeGetToken(jsonObj, "local_to_world_transform"),
															  worldToLocal, localToWorld);
			newPrimitive->SetTransformAndInverse(worldToLocal, localToWorld);
		}
	}
	
	return newPrimitive;
}
