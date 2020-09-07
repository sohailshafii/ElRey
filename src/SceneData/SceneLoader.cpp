#include "SceneLoader.h"
#include "ThirdParty/nlohmann/json.hpp"
#include <fstream>
#include <exception>
#include <iostream>
#include <sstream>
#include "Materials/Material.h"
#include "SceneData/Scene.h"
#include "SceneData/CommonLoaderFunctions.h"
#include "SceneData/PrimitiveLoader.h"
#include "SceneData/Light.h"
#include "SceneData/AmbientLight.h"
#include "SceneData/AmbientLightOccluder.h"
#include "SceneData/DirectionalLight.h"
#include "SceneData/PointLight.h"
#include "SceneData/AreaLight.h"
#include "SceneData/EnvironmentLight.h"
#include "Cameras/Camera.h"
#include "Cameras/PinholeCamera.h"
#include "Cameras/FisheyeCamera.h"
#include "Cameras/OrthographicCamera.h"
#include "Cameras/SphericalPanoramicCamera.h"
#include "Cameras/ThinLensCamera.h"
#include "Primitives/CompoundObject.h"

static Camera* CreateCamera(const nlohmann::json& jsonObj);
static Light* CreateLight(const nlohmann::json& jsonObj);

void SceneLoader::DeserializeJSONFileIntoScene(Scene* scene,
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
		
		PrimitiveLoader::AddPrimitivesToScene(scene, jsonObj["objects"]);

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
		 numLights; lightIndex++) {
		Light* currentLight = scene->GetLight(lightIndex);
		// find primitive if current light needs to be sampled
		if (currentLight->IsAreaLight()) {
			std::string const & primName = currentLight->GetPrimitiveName();
			Primitive* foundPrimitive = scene->FindPrimitiveByName(primName);
			if (foundPrimitive != nullptr) {
				currentLight->SetPrimitive(foundPrimitive);
			}
		}
	}
}

static Camera* CreateCamera(const nlohmann::json& jsonObj) {
	std::string cameraType = CommonLoaderFunctions::SafeGetToken(jsonObj, "type");
	std::cout << "Camera type specified: " << cameraType << ".\n";
	Camera* mainCamera = nullptr;

	auto eyePosition = CommonLoaderFunctions::SafeGetToken(jsonObj, "eye_position");
	auto lookAtPosition = CommonLoaderFunctions::SafeGetToken(jsonObj, "look_at_position");
	int numColumnsPixels = CommonLoaderFunctions::SafeGetToken(jsonObj, "num_columns_pixels");
	int numRowsPixels = CommonLoaderFunctions::SafeGetToken(jsonObj, "num_rows_pixels");
	float viewPlaneWidth = CommonLoaderFunctions::SafeGetToken(jsonObj, "view_plane_width");
	RandomSamplerType randomSamplerType;
	int numRandomSamples, numRandomSets;
	CommonLoaderFunctions::SetUpRandomSampler(jsonObj, randomSamplerType,
											  numRandomSamples, numRandomSets);
	
	std::cout.precision(5);
	auto upVector = CommonLoaderFunctions::SafeGetToken(jsonObj, "up_vector");
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
		float psiMax = CommonLoaderFunctions::SafeGetToken(jsonObj, "psi_max");
		float exposureTime = CommonLoaderFunctions::SafeGetToken(jsonObj, "exposure_time");

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
		float psiMax = CommonLoaderFunctions::SafeGetToken(jsonObj, "psi_max");
		float lambdaMax = CommonLoaderFunctions::SafeGetToken(jsonObj, "lambda_max");
		float exposureTime = CommonLoaderFunctions::SafeGetToken(jsonObj, "exposure_time");
		mainCamera = new SphericalPanoramicCamera(Point3((float)eyePosition[0], (float)eyePosition[1],
														 (float)eyePosition[2]),
												  Point3((float)lookAtPosition[0], (float)lookAtPosition[1],
														 (float)lookAtPosition[2]), numColumnsPixels,
												  numRowsPixels, viewPlaneWidth, viewPlaneHeight,
												  Vector3((float)upVector[0], (float)upVector[1], (float)upVector[2]),
												  randomSamplerType, numRandomSamples, numRandomSets, psiMax, lambdaMax, exposureTime);
	}
	else if (cameraType == "thin_lens") {
		float lensRadius = CommonLoaderFunctions::SafeGetToken(jsonObj, "lens_radius");
		float focalPlaneDistance = CommonLoaderFunctions::SafeGetToken(jsonObj, "focal_plane_distance");
		float exposureTime = CommonLoaderFunctions::SafeGetToken(jsonObj, "exposure_time");
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

Light* CreateLight(const nlohmann::json& jsonObj) {
	std::string lightType = CommonLoaderFunctions::SafeGetToken(jsonObj, "type");
	Light* newLight = nullptr;
	
	if (lightType == "ambient") {
		auto radiance = CommonLoaderFunctions::SafeGetToken(jsonObj, "radiance");
		float radianceScale = CommonLoaderFunctions::SafeGetToken(jsonObj, "radiance_scale");
		newLight = new AmbientLight(Color3((float)radiance[0], (float)radiance[1],
			(float)radiance[2]), radianceScale);
	}
	else if (lightType == "ambient_occluder") {
		auto radiance = CommonLoaderFunctions::SafeGetToken(jsonObj, "radiance");
		float radianceScale = CommonLoaderFunctions::SafeGetToken(jsonObj, "radiance_scale");
		float minAmount = CommonLoaderFunctions::SafeGetToken(jsonObj, "min_amount");
		
		RandomSamplerType randomSamplerType;
		int numRandomSamples, numRandomSets;
		CommonLoaderFunctions::SetUpRandomSampler(jsonObj, randomSamplerType, 										numRandomSamples, numRandomSets);
		newLight = new AmbientLightOccluder(Color3((float)radiance[0], (float)radiance[1],
			(float)radiance[2]), radianceScale, minAmount, randomSamplerType, numRandomSamples, numRandomSets);
	}
	else if (lightType == "directional") {
		auto direction = CommonLoaderFunctions::SafeGetToken(jsonObj, "direction");
		auto radiance = CommonLoaderFunctions::SafeGetToken(jsonObj, "radiance");
		float radianceScale = CommonLoaderFunctions::SafeGetToken(jsonObj, "radiance_scale");
		bool castsShadows = CommonLoaderFunctions::SafeGetToken(jsonObj, "casts_shadows");
		newLight = new DirectionalLight(castsShadows, Vector3((float)direction[0], (float)direction[1],
			(float)direction[2]), Color3((float)radiance[0], (float)radiance[1],
			(float)radiance[2]), radianceScale);
	}
	else if (lightType == "point") {
		auto position = CommonLoaderFunctions::SafeGetToken(jsonObj, "position");
		auto radiance = CommonLoaderFunctions::SafeGetToken(jsonObj, "radiance");
		float radianceScale = CommonLoaderFunctions::SafeGetToken(jsonObj, "radiance_scale");
		bool castsShadows = CommonLoaderFunctions::SafeGetToken(jsonObj, "casts_shadows");
		newLight = new PointLight(castsShadows, Point3((float)position[0], (float)position[1],
			(float)position[2]), Color3((float)radiance[0], (float)radiance[1],
			(float)radiance[2]), radianceScale);
	}
	else if (lightType == "area_light") {
		std::string primitiveName = CommonLoaderFunctions::SafeGetToken(jsonObj, "primitive_name");
		bool castsShadows = CommonLoaderFunctions::SafeGetToken(jsonObj, "casts_shadows");
		newLight = new AreaLight(castsShadows, primitiveName);
	}
	else if (lightType == "environment") {
		RandomSamplerType randomSamplerType;
		int numRandomSamples, numRandomSets;
		CommonLoaderFunctions::SetUpRandomSampler(jsonObj, randomSamplerType,
												  numRandomSamples, numRandomSets);
		auto materialNode = CommonLoaderFunctions::SafeGetToken(jsonObj, "material");
		bool castsShadows = CommonLoaderFunctions::SafeGetToken(jsonObj, "casts_shadows");
		std::shared_ptr<Material> objMaterial =
			CommonLoaderFunctions::CreateMaterial(materialNode);
		newLight = new EnvironmentLight(castsShadows, randomSamplerType,
										numRandomSamples,
										numRandomSets,
										objMaterial);
	}
	return newLight;
}
