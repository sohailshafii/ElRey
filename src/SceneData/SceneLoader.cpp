#include "SceneLoader.h"
#include "ThirdParty/nlohmann/json.hpp"
#include <fstream>
#include <exception>
#include <iostream>
#include <sstream>
#include "Sampling/SamplerCreator.h"
#include "SceneData/Scene.h"
#include "Primitives/Sphere.h"
#include "Primitives/Plane.h"
#include "Primitives/Rectangle.h"
#include "Materials/LambertianMaterial.h"
#include "Materials/PhongMaterial.h"
#include "Materials/SimpleEmissiveMaterial.h"
#include "SceneData/Light.h"
#include "SceneData/AmbientLight.h"
#include "SceneData/AmbientLightOccluder.h"
#include "SceneData/DirectionalLight.h"
#include "SceneData/PointLight.h"
#include "SceneData/AreaLight.h"
#include "Cameras/Camera.h"
#include "Cameras/PinholeCamera.h"
#include "Cameras/FisheyeCamera.h"
#include "Cameras/OrthographicCamera.h"
#include "Cameras/SphericalPanoramicCamera.h"
#include "Cameras/ThinLensCamera.h"

static void SetUpRandomSampler(const nlohmann::json& jsonObj,
							   RandomSamplerType& randomSamplerType,
							   int &numRandomSamples, int &numRandomSets);
static Camera* CreateCamera(const nlohmann::json& jsonObj);
static Primitive* CreatePrimitive(const nlohmann::json& jsonObj);
static std::shared_ptr<Material> CreateMaterial(const nlohmann::json& jsonObj);
static Light* CreateLight(const nlohmann::json& jsonObj);

void SceneLoader::DeserializeJSONFileIntoScene(class Scene* scene,
											   const std::string &jsonFilePath) {
	try {
		std::ifstream jsonFile(jsonFilePath);
		nlohmann::json jsonObj;
	
		jsonFile >> jsonObj;
		
		nlohmann::json sceneSettings = jsonObj["scene_settings"];
		std::string navigationToken = sceneSettings["navigation_type"];
		scene->SetAllowNavigation(navigationToken == "fps");
		nlohmann::json cameraSettings = sceneSettings["camera"];
		Camera* mainCamera = CreateCamera(cameraSettings);
		scene->SetCamera(mainCamera);
		
		nlohmann::json objectsArray = jsonObj["objects"];
		for(auto& element : objectsArray.items()) {
			Primitive* newPrimitive = CreatePrimitive(element.value());
			if (newPrimitive != nullptr) {
				scene->AddPrimitive(newPrimitive);
			}
		}

		nlohmann::json lightsArray = jsonObj["lights"];
		for (auto& element : lightsArray.items()) {
			Light* newLight = CreateLight(element.value());
			if (newLight != nullptr) {
				scene->AddLight(newLight);
			}
		}
	}
	catch(const std::exception& e) {
		std::stringstream exceptionMsg;
		exceptionMsg << "Could not deserialize JSON file: " << jsonFilePath
			<< ". Reason: " << e.what() << std::endl;
		throw exceptionMsg;
	}

	unsigned int numLights = scene->GetNumLights();
	for (unsigned int lightIndex = 0; lightIndex <
		 numLights; lightIndex++)
	{
		Light* currentLight = scene->GetLight(lightIndex);
		// find primitive if current light needs to be sampled
		if (currentLight->NeedsToBeSampled())
		{
			std::string const & primName = currentLight->GetPrimitiveName();
			Primitive* foundPrimitive = scene->FindPrimitiveByName(primName);
			if (foundPrimitive != nullptr)
			{
				currentLight->SetPrimitive(foundPrimitive);
			}
		}
	}
}

static inline nlohmann::json SafeGetToken(const nlohmann::json& jsonObj,
									const std::string& key) {
	if (jsonObj.find(key) != jsonObj.end()) {
		return jsonObj[key];
	}
	std::stringstream exceptionMsg;
	exceptionMsg << "Could not find key: " << key
		<< " in JSON object: " << jsonObj << ".\n";
	throw exceptionMsg;
}

static void SetUpRandomSampler(const nlohmann::json& jsonObj,
							   RandomSamplerType& randomSamplerType,
							   int &numRandomSamples, int &numRandomSets) {
	randomSamplerType = None;
	std::string samplerTypeToken = SafeGetToken(jsonObj, "random_sampler_type");
	if (samplerTypeToken == "random") {
		randomSamplerType = Random;
	}
	else if (samplerTypeToken == "regular") {
		randomSamplerType = Regular;
	}
	else if (samplerTypeToken == "jittered") {
		randomSamplerType = Jittered;
	}
	else if (samplerTypeToken == "nrooks") {
		randomSamplerType = NRooks;
	}
	else if (samplerTypeToken == "multijittered") {
		randomSamplerType = MultiJittered;
	}
	else if (samplerTypeToken == "none") {
		// es ok
	}
	else {
		std::stringstream exceptionMsg;
		exceptionMsg << "Cannot understand sampler type specified: " <<
			samplerTypeToken.c_str() << ".\n";
		throw exceptionMsg;
	}
	numRandomSamples = SafeGetToken(jsonObj, "num_random_samples");
	numRandomSets = SafeGetToken(jsonObj, "num_random_sets");
}

static Camera* CreateCamera(const nlohmann::json& jsonObj) {
	std::string cameraType = SafeGetToken(jsonObj, "type");
	std::cout << "Camera type specified: " << cameraType << ".\n";
	Camera* mainCamera = nullptr;

	auto eyePosition = SafeGetToken(jsonObj, "eye_position");
	auto lookAtPosition = SafeGetToken(jsonObj, "look_at_position");
	int numColumnsPixels = SafeGetToken(jsonObj, "num_columns_pixels");
	int numRowsPixels = SafeGetToken(jsonObj, "num_rows_pixels");
	float viewPlaneWidth = SafeGetToken(jsonObj, "view_plane_width");
	RandomSamplerType randomSamplerType;
	int numRandomSamples, numRandomSets;
	SetUpRandomSampler(jsonObj, randomSamplerType, numRandomSamples,
					   numRandomSets);
	
	std::cout.precision(5);
	auto upVector = SafeGetToken(jsonObj, "up_vector");
	float viewPlaneHeight = viewPlaneWidth * (float)numRowsPixels / (float)numColumnsPixels;
	float rowHeight = viewPlaneHeight / numRowsPixels;
	float colWidth = viewPlaneWidth / numColumnsPixels;
	std::cout << "Image plane dimensions: " << viewPlaneWidth << " x "
		<< viewPlaneHeight << " Row height: " << rowHeight << ", col width: "
		<< colWidth << ".\n";
	
	if (cameraType == "pinhole") {
		mainCamera = new PinholeCamera(Point3((float)eyePosition[0], (float)eyePosition[1],
											  (float)eyePosition[2]),
									   Point3((float)lookAtPosition[0], (float)lookAtPosition[1],
											  (float)lookAtPosition[2]), numColumnsPixels,
									   numRowsPixels, viewPlaneWidth, viewPlaneHeight,
									   Vector3((float)upVector[0], (float)upVector[1], (float)upVector[2]), randomSamplerType,
									   numRandomSamples, numRandomSets);
	}
	else if (cameraType == "fish_eye") {
		float psiMax = SafeGetToken(jsonObj, "psi_max");
		float exposureTime = SafeGetToken(jsonObj, "exposure_time");

		mainCamera = new FisheyeCamera(Point3((float)eyePosition[0], (float)eyePosition[1],
											  (float)eyePosition[2]),
									   Point3((float)lookAtPosition[0], (float)lookAtPosition[1],
											  (float)lookAtPosition[2]), numColumnsPixels,
									   numRowsPixels, viewPlaneWidth, viewPlaneHeight, Vector3((float)upVector[0], (float)upVector[1], (float)upVector[2]),
									   randomSamplerType, numRandomSamples, numRandomSets, psiMax,
									   exposureTime);
	}
	else if (cameraType == "ortho") {
		mainCamera = new OrthographicCamera(Point3((float)eyePosition[0], (float)eyePosition[1],
											(float)eyePosition[2]),
											Point3((float)lookAtPosition[0], (float)lookAtPosition[1],
											(float)lookAtPosition[2]), numColumnsPixels, numRowsPixels,
											viewPlaneWidth, viewPlaneHeight,
											Vector3((float)upVector[0], (float)upVector[1], (float)upVector[2]),
											randomSamplerType, numRandomSamples, numRandomSets);
	}
	else if (cameraType == "spherical_pano") {
		float psiMax = SafeGetToken(jsonObj, "psi_max");
		float lambdaMax = SafeGetToken(jsonObj, "lambda_max");
		float exposureTime = SafeGetToken(jsonObj, "exposure_time");
		mainCamera = new SphericalPanoramicCamera(Point3((float)eyePosition[0], (float)eyePosition[1],
														 (float)eyePosition[2]),
												  Point3((float)lookAtPosition[0], (float)lookAtPosition[1],
														 (float)lookAtPosition[2]), numColumnsPixels,
												  numRowsPixels, viewPlaneWidth, viewPlaneHeight,
												  Vector3((float)upVector[0], (float)upVector[1], (float)upVector[2]),
												  randomSamplerType, numRandomSamples, numRandomSets, psiMax, lambdaMax, exposureTime);
	}
	else if (cameraType == "thin_lens") {
		float lensRadius = SafeGetToken(jsonObj, "lens_radius");
		float focalPlaneDistance = SafeGetToken(jsonObj, "focal_plane_distance");
		float exposureTime = SafeGetToken(jsonObj, "exposure_time");
		mainCamera = new ThinLensCamera(Point3((float)eyePosition[0], (float)eyePosition[1],
											   (float)eyePosition[2]),
										Point3((float)lookAtPosition[0], (float)lookAtPosition[1],
											   (float)lookAtPosition[2]), numColumnsPixels,
										numRowsPixels, viewPlaneWidth, viewPlaneHeight,
										Vector3((float)upVector[0], (float)upVector[1], (float)upVector[2]), randomSamplerType,
										numRandomSamples, numRandomSets, lensRadius, focalPlaneDistance,
										exposureTime);
	}
	else {
		std::stringstream exceptionMsg;
		exceptionMsg << "Could not recognize camera type: " << cameraType
			<< ".\n";
		throw exceptionMsg;
	}
	return mainCamera;
}

static Primitive* CreatePrimitive(const nlohmann::json& jsonObj) {
	std::string primitiveType = SafeGetToken(jsonObj, "type");
	Primitive* newPrimitive = nullptr;
	if (primitiveType == "plane") {
		auto planeOrigin = SafeGetToken(jsonObj, "position");
		auto planeNormal = SafeGetToken(jsonObj, "normal");
		auto materialNode = SafeGetToken(jsonObj, "material");
		std::string objectName = SafeGetToken(jsonObj, "name");

		std::shared_ptr<Material> objMaterial = CreateMaterial(materialNode);
		// TODO: fix error here
		newPrimitive = new Plane(
			Point3((float)planeOrigin[0],(float)planeOrigin[1],
				(float)planeOrigin[2]),
			Vector3((float)planeNormal[0],
				(float)planeNormal[1],(float)planeNormal[2]),
			objMaterial, objectName);
	}
	else if (primitiveType == "sphere") {
		auto sphereOrigin = SafeGetToken(jsonObj, "position");
		float sphereRadius = SafeGetToken(jsonObj, "radius");
		auto materialNode = SafeGetToken(jsonObj, "material");
		std::string objectName = SafeGetToken(jsonObj, "name");
		
		std::shared_ptr<Material> objMaterial = CreateMaterial(materialNode);
		// TODO: fix error here
		newPrimitive = new Sphere(
			Point3((float)sphereOrigin[0],(float)sphereOrigin[1],
					(float)sphereOrigin[2]),
			sphereRadius, objMaterial, objectName);
	}
	else if (primitiveType == "rectangle") {
		std::string objectName = SafeGetToken(jsonObj, "name");
		auto origin = SafeGetToken(jsonObj, "origin");
		auto sideVec1 = SafeGetToken(jsonObj, "side_vec_1");
		auto sideVec2 = SafeGetToken(jsonObj, "side_vec_2");
		auto materialNode = SafeGetToken(jsonObj, "material");
		auto samplerNode = SafeGetToken(jsonObj, "sampler");

		std::shared_ptr<Material> objMaterial = CreateMaterial(materialNode);

		RandomSamplerType randomSamplerType;
		int numRandomSamples, numRandomSets;
		SetUpRandomSampler(samplerNode, randomSamplerType, numRandomSamples,
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
	else {
		std::stringstream exceptionMsg;
		exceptionMsg << "Could not recognize type: " << primitiveType
			<< ".\n";
		throw exceptionMsg;
	}
	return newPrimitive;
}

std::shared_ptr<Material> CreateMaterial(const nlohmann::json& jsonObj) {
	std::shared_ptr<Material> newMaterial;
	std::string primitiveType = SafeGetToken(jsonObj, "type");
	if (primitiveType == "lambertian") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		auto colorObj = SafeGetToken(jsonObj, "color");
		// TODO: should BRDFs have four-component colors?
		newMaterial = std::make_shared<LambertianMaterial>(kA, kD,
			Color3(colorObj[0], colorObj[1],
				colorObj[2]));
	}
	else if (primitiveType == "phong") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		float kS = SafeGetToken(jsonObj, "ks");
		float exponent = SafeGetToken(jsonObj, "exponent");
		auto colorObj = SafeGetToken(jsonObj, "color");
		auto ksColor = SafeGetToken(jsonObj, "ks_color");
		// TODO: should BRDFs have four-component colors?
		newMaterial = std::make_shared<PhongMaterial>(kA, kD, kS, exponent,
			Color3(colorObj[0], colorObj[1],
				colorObj[2]),
			Color3(ksColor[0], ksColor[1],
				ksColor[2]));
	}
	else if (primitiveType == "simple_emissive") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		auto colorObj = SafeGetToken(jsonObj, "color");
		newMaterial = std::make_shared<SimpleEmissiveMaterial>
			(kA, kD, Color3(colorObj[0], colorObj[1], colorObj[2]));
	}
	return newMaterial;
}

Light* CreateLight(const nlohmann::json& jsonObj) {
	std::string lightType = SafeGetToken(jsonObj, "type");
	Light* newLight = nullptr;
	std::string primitiveType = SafeGetToken(jsonObj, "type");
	if (primitiveType == "ambient") {
		auto radiance = SafeGetToken(jsonObj, "radiance");
		float radianceScale = SafeGetToken(jsonObj, "radiance_scale");
		newLight = new AmbientLight(Color3((float)radiance[0], (float)radiance[1],
			(float)radiance[2]), radianceScale);
	}
	else if (primitiveType == "ambient_occluder") {
		auto radiance = SafeGetToken(jsonObj, "radiance");
		float radianceScale = SafeGetToken(jsonObj, "radiance_scale");
		float minAmount = SafeGetToken(jsonObj, "min_amount");
		
		RandomSamplerType randomSamplerType;
		int numRandomSamples, numRandomSets;
		SetUpRandomSampler(jsonObj, randomSamplerType, numRandomSamples,
						   numRandomSets);
		newLight = new AmbientLightOccluder(Color3((float)radiance[0], (float)radiance[1],
			(float)radiance[2]), radianceScale, minAmount, randomSamplerType, numRandomSamples, numRandomSets);
	}
	else if (primitiveType == "directional") {
		auto direction = SafeGetToken(jsonObj, "direction");
		auto radiance = SafeGetToken(jsonObj, "radiance");
		float radianceScale = SafeGetToken(jsonObj, "radiance_scale");
		bool castsShadows = SafeGetToken(jsonObj, "casts_shadows");
		newLight = new DirectionalLight(castsShadows, Vector3((float)direction[0], (float)direction[1],
			(float)direction[2]), Color3((float)radiance[0], (float)radiance[1],
			(float)radiance[2]), radianceScale);
	}
	else if (primitiveType == "point") {
		auto position = SafeGetToken(jsonObj, "position");
		auto radiance = SafeGetToken(jsonObj, "radiance");
		float radianceScale = SafeGetToken(jsonObj, "radiance_scale");
		bool castsShadows = SafeGetToken(jsonObj, "casts_shadows");
		newLight = new PointLight(castsShadows, Point3((float)position[0], (float)position[1],
			(float)position[2]), Color3((float)radiance[0], (float)radiance[1],
			(float)radiance[2]), radianceScale);
	}
	else if (primitiveType == "area_light") {
		std::string primitiveName = SafeGetToken(jsonObj, "primitive_name");
		bool castsShadows = SafeGetToken(jsonObj, "casts_shadows");
		//newLight = new AreaLight(castsShadows, primitiveName);
	}
	return newLight;
}
