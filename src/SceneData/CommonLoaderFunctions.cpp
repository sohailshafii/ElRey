#include "SceneData/CommonLoaderFunctions.h"
#include "Materials/LambertianMaterial.h"
#include "Materials/PhongMaterial.h"
#include "Materials/SimpleEmissiveMaterial.h"
#include "Materials/ReflectiveMaterial.h"
#include "Materials/GlossySpecularMaterial.h"
#include "Materials/TransparentMaterial.h"
#include "Materials/DielectricMaterial.h"
#include "Materials/Texturing/SingleColorTex.h"
#include "Materials/Texturing/ImageTextureRegistry.h"
#include "Materials/Texturing/InstanceTextureRegistry.h"
#include "Materials/Texturing/Mapping/NullMapping.h"
#include "Materials/Texturing/Mapping/RectangularMapping.h"
#include "Materials/Texturing/Mapping/SphericalMapping.h"
#include "Materials/Texturing/PlaneCheckerTex.h"
#include "Materials/Texturing/Mapping/ModelCoordMapping.h"
#include "Materials/Texturing/NoiseTexture.h"
#include "Materials/Texturing/NoiseRampTexture.h"
#include "Math/LinearNoiseFunction.h"
#include "Math/CubicNoiseFunction.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <cassert>

nlohmann::json CommonLoaderFunctions::SafeGetToken(nlohmann::json const & jsonObj,
												   std::string const & key) {
	if (HasKey(jsonObj, key)) {
		return jsonObj[key];
	}
	std::stringstream exceptionMsg;
	exceptionMsg << "Could not find key: " << key
		<< " in JSON object: " << jsonObj << ".\n";
	throw exceptionMsg;
}

bool CommonLoaderFunctions::HasKey(nlohmann::json const & jsonObj,
								   std::string const & key) {
	return (jsonObj.find(key) != jsonObj.end());
}

std::shared_ptr<Material> CommonLoaderFunctions::CreateMaterial(nlohmann::json const & topmostJsonNode,
																nlohmann::json const & jsonObj) {
	std::shared_ptr<Material> newMaterial;

	std::string primitiveType = SafeGetToken(jsonObj, "type");
	
	if (primitiveType == "lambertian") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		
		newMaterial = std::make_shared<LambertianMaterial>(kA, kD,
														   CreateTexture(topmostJsonNode,
																		 jsonObj,
																		 "color"));
	}
	else if (primitiveType == "phong") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		float kS = SafeGetToken(jsonObj, "ks");
		float exponent = SafeGetToken(jsonObj, "exponent");
		
		newMaterial = std::make_shared<PhongMaterial>(kA, kD, kS, exponent,
													  CreateTexture(topmostJsonNode,
																	jsonObj,
																	"color"),
													  CreateTexture(topmostJsonNode,
																	jsonObj,
																	"ks_color"));
	}
	else if (primitiveType == "reflective") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		float kS = SafeGetToken(jsonObj, "ks");
		float cR = SafeGetToken(jsonObj, "cr");
		float kR = SafeGetToken(jsonObj, "kr");
		float exponent = SafeGetToken(jsonObj, "exponent");
		
		newMaterial = std::make_shared<ReflectiveMaterial>(kA, kD, kS, exponent,
														   CreateTexture(topmostJsonNode,
																		 jsonObj,
																		"color"),
														   CreateTexture(topmostJsonNode,
																		 jsonObj,
																		"ks_color"),
																		cR, kR);
	}
	else if (primitiveType == "transparent") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		float kS = SafeGetToken(jsonObj, "ks");
		float cR = SafeGetToken(jsonObj, "cr");
		float kR = SafeGetToken(jsonObj, "kr");
		float kt = SafeGetToken(jsonObj, "kt");
		float eta = SafeGetToken(jsonObj, "eta");
		float exponent = SafeGetToken(jsonObj, "exponent");
		
		newMaterial = std::make_shared<TransparentMaterial>(kA, kD, kS, exponent,
															CreateTexture(topmostJsonNode,
																		  jsonObj,
																	"color"),
															CreateTexture(topmostJsonNode,
																		  jsonObj,
																	"ks_color"),
															cR, kR, eta, kt);
	}
	else if (primitiveType == "dielectric") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		float kS = SafeGetToken(jsonObj, "ks");
		float etaIn = SafeGetToken(jsonObj, "eta_in");
		float etaOut = SafeGetToken(jsonObj, "eta_out");
		float exponent = SafeGetToken(jsonObj, "exponent");
		
		auto cfIn = SafeGetToken(jsonObj, "cf_in");
		auto cfOut = SafeGetToken(jsonObj, "cf_out");
		
		newMaterial = std::make_shared<DielectricMaterial>(kA, kD, kS, exponent,
														   CreateTexture(topmostJsonNode,
																		 jsonObj,
																		 "color"),
														   CreateTexture(topmostJsonNode,
																		 jsonObj,
																		 "ks_color"),
														   etaIn, etaOut,
														   Color3(cfIn[0], cfIn[1], cfIn[2]),
														   Color3(cfOut[0], cfOut[1], cfOut[2]));
	}
	else if (primitiveType == "simple_emissive") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		
		newMaterial = std::make_shared<SimpleEmissiveMaterial>
			(kA, kD, CreateTexture(topmostJsonNode,
								   jsonObj, "color"));
	}
	else if (primitiveType == "glossy_specular") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		float kS = SafeGetToken(jsonObj, "ks");
		float cR = SafeGetToken(jsonObj, "cr");
		float kR = SafeGetToken(jsonObj, "kr");
		float exponent = SafeGetToken(jsonObj, "exponent");
		
		newMaterial = std::make_shared<GlossySpecularMaterial>(kA, kD, kS,
															   exponent,
															   CreateTexture(topmostJsonNode,
																			 jsonObj,
																			 "color"),
															   CreateTexture(topmostJsonNode,
																			 jsonObj,
																			 "ks_color"),
															   cR, kR);
	}
	
	if (HasKey(jsonObj, "sampler")) {
		nlohmann::json samplerJson = SafeGetToken(jsonObj, "sampler");
		RandomSamplerType randomSamplerType;
		int numRandomSamples;
		int numRandomSets;
		SetUpRandomSampler(samplerJson, randomSamplerType,
						   numRandomSamples, numRandomSets);
		GenericSampler* genericSampler =
			SamplerCreator::CreatorSampler(randomSamplerType,
										   numRandomSamples, numRandomSets);
		newMaterial->SetSampler(genericSampler);
	}
	
	return newMaterial;
}

std::shared_ptr<AbstractTexture> CommonLoaderFunctions::CreateTexture(nlohmann::json const & topmostJsonNode,
																	  nlohmann::json const & jsonObject,
																	  std::string const & colorKey) {
	auto colorObj = SafeGetToken(jsonObject, colorKey);
	std::string type = SafeGetToken(jsonObject, "type");
	if (HasKey(colorObj, "texture_instance")) {
		return CreateTextureInstance(topmostJsonNode, colorObj);
	}
	else {
		return CreateTextureFromNode(colorObj);
	}
}

std::shared_ptr<AbstractTexture> CommonLoaderFunctions::CreateTextureFromNode(
																			  nlohmann::json const & jsonObject) {
	std::shared_ptr<AbstractTexture> createdTex = nullptr;
	if (HasKey(jsonObject, "image_texture")) {
		auto imageTextureObj = SafeGetToken(jsonObject, "image_texture");
		std::string filePath = SafeGetToken(imageTextureObj, "file_path");
		std::string sampleTypeStr = SafeGetToken(imageTextureObj, "sample_type");
		std::string name = SafeGetToken(imageTextureObj, "name");
		AbstractTexture::SamplingType sampleType = AbstractTexture::SamplingType::Nearest;
		if (sampleTypeStr == "bilinear") {
			sampleType = AbstractTexture::SamplingType::Bilinear;
		}
		else {
			sampleType = AbstractTexture::SamplingType::Trilinear;
		}
		std::shared_ptr<MappingLayer> mappingLayer = CreateMappingLayer(imageTextureObj);
		createdTex = ImageTextureRegistry::GetInstance().GetTextureForPath(filePath,
																		   mappingLayer,
																		   sampleType,
																		   name);
	}
	else if (HasKey(jsonObject, "plane_checker")) {
		auto planeChecker = SafeGetToken(jsonObject, "plane_checker");
		unsigned int checkerSize = SafeGetToken(planeChecker, "checker_size");
		unsigned int outlineWidth = SafeGetToken(planeChecker, "outline_width");
		auto inColor = SafeGetToken(planeChecker, "in_color");
		auto outColor = SafeGetToken(planeChecker, "out_color");
		auto outlineColor = SafeGetToken(planeChecker, "outline_color");
		std::string name = SafeGetToken(planeChecker, "name");
		createdTex = std::make_shared<PlaneCheckerTex>(PlaneCheckerTex(checkerSize, outlineWidth,
									 Color3(inColor[0], inColor[1], inColor[2]),
									 Color3(outColor[0], outColor[1], outColor[2]),
									 Color3(outlineColor[0], outlineColor[1], outlineColor[2]),
																	   name));
	}
	else if (HasKey(jsonObject, "noise_texture")) {
		auto noiseTextureToken = SafeGetToken(jsonObject, "noise_texture");
		auto minColorToken = SafeGetToken(noiseTextureToken, "min_color");
		auto maxColorToken = SafeGetToken(noiseTextureToken, "max_color");
		auto colorTypeToken = SafeGetToken(noiseTextureToken, "color_type");
		float expansionNumber = SafeGetToken(noiseTextureToken, "expansion_number");
		std::string functionTypeToken = SafeGetToken(noiseTextureToken, "function_type");
		std::string name = SafeGetToken(noiseTextureToken, "name");
		
		Color3 minColor((float)minColorToken[0], (float)minColorToken[1],
						(float)minColorToken[2]);
		Color3 maxColor((float)maxColorToken[0], (float)maxColorToken[1],
						(float)maxColorToken[2]);
		std::shared_ptr<NoiseFunction> noiseFunction = CreateNoiseFunction(noiseTextureToken);
		
		//Wrapping = 0, NoWrapping, Vector
		NoiseTexture::ColorType colorType = NoiseTexture::ColorType::Wrapping;
		if (colorTypeToken == "wrapping") {
			colorTypeToken = NoiseTexture::ColorType::Wrapping;
		}
		else if (colorTypeToken == "no_wrapping") {
			colorTypeToken = NoiseTexture::ColorType::NoWrapping;
		}
		else if (colorTypeToken == "vector") {
			colorTypeToken = NoiseTexture::ColorType::Vector;
		}
		
		NoiseTexture::FunctionType functionType = NoiseTexture::FunctionType::ValueInterp;
		if (functionTypeToken == "value_interp")
		{
			functionType = NoiseTexture::FunctionType::ValueInterp;
		}
		else if (functionTypeToken == "vector_interp")
		{
			functionType = NoiseTexture::FunctionType::VectorInterp;
		}
		else if (functionTypeToken == "value_fbm")
		{
			functionType = NoiseTexture::FunctionType::ValueFBM;
		}
		else if (functionTypeToken == "vector_fbm")
		{
			functionType = NoiseTexture::FunctionType::VectorFBM;
		}
		else if (functionTypeToken == "value_turb_fbm")
		{
			functionType = NoiseTexture::FunctionType::ValueTurbFBM;
		}
		else if (functionTypeToken == "value_fractal_sum")
		{
			functionType = NoiseTexture::FunctionType::ValueFractalSum;
		}
		else if (functionTypeToken == "vector_fractal_sum")
		{
			functionType = NoiseTexture::FunctionType::VectorFractalSum;
		}
		else
		{
			std::stringstream exceptionMsg;
			exceptionMsg << "Could not understand noise function type: " << functionTypeToken
				<< " in JSON object: " << noiseTextureToken << ".\n";
			throw exceptionMsg;
		}
		
		createdTex =
		std::make_shared<NoiseTexture>(minColor,
									   maxColor,
									   noiseFunction,
									   colorTypeToken,
									   expansionNumber,
									   functionType,
									   name);
	}
	else if (HasKey(jsonObject, "noise_ramp_texture")) {
		auto noiseTextureToken = SafeGetToken(jsonObject, "noise_ramp_texture");
		float amplitude = SafeGetToken(noiseTextureToken, "amplitude");
		std::string filePath = SafeGetToken(noiseTextureToken, "file_path");
		std::string name = SafeGetToken(noiseTextureToken, "name");
		std::shared_ptr<NoiseFunction> noiseFunction = CreateNoiseFunction(noiseTextureToken);
		std::shared_ptr<TextureData> textureData =
			ImageTextureRegistry::GetInstance().GetTextureData(filePath);
		
		createdTex =
		std::make_shared<NoiseRampTexture>(textureData, noiseFunction, amplitude, name);
	}
	else {
		static int colorCounter = 0;
		createdTex = std::make_shared<SingleColorTex>(std::make_shared<NullMapping>(),
													  Color3(jsonObject[0], jsonObject[1],
															 jsonObject[2]),
													  ImageTexture::SamplingType::Nearest,
													  "SingleColor-"+std::to_string(colorCounter));
	}
	return createdTex;
}

std::shared_ptr<AbstractTexture> CommonLoaderFunctions::CreateTextureInstance(nlohmann::json const & topmostJsonNode,
													   nlohmann::json const & jsonObject) {
	std::string textureInstanceName = SafeGetToken(jsonObject, "texture_instance");
	std::shared_ptr<AbstractTexture> registeredTexture =
	InstanceTextureRegistry::GetInstance().GetTextureForName(textureInstanceName);
	
	if (registeredTexture == nullptr) {
		auto instancesToken = SafeGetToken(topmostJsonNode, "texture_instances");
		for(auto const & element : instancesToken.items()) {
			auto const & value = element.value();
			std::string name = SafeGetToken(value, "name");
			if (name == textureInstanceName) {
				registeredTexture = CreateTextureFromNode(value);
				InstanceTextureRegistry::GetInstance().AddTexture(registeredTexture);
			}
		}
	}

	return registeredTexture;
}

std::shared_ptr<NoiseFunction> CommonLoaderFunctions::CreateNoiseFunction(
										nlohmann::json const &noiseTextureToken) {
	std::shared_ptr<NoiseFunction> noiseFunction = nullptr;
	std::string noiseFunctionClass = SafeGetToken(noiseTextureToken, "noise_function_class");
	unsigned int numOctaves = SafeGetToken(noiseTextureToken, "num_octaves");
	float gain = SafeGetToken(noiseTextureToken, "gain");
	float lacunarity = SafeGetToken(noiseTextureToken, "lacuranity");
	float startingFrequency = SafeGetToken(noiseTextureToken, "starting_frequency");
	if (noiseFunctionClass == "linear_noise_function") {
		noiseFunction = std::make_shared<LinearNoiseFunction>(numOctaves,
															  lacunarity,
															  gain,
															  startingFrequency);
	}
	else if (noiseFunctionClass == "cubic_noise_function") {
		noiseFunction = std::make_shared<CubicNoiseFunction>(numOctaves,
															 lacunarity,
															 gain,
															 startingFrequency);
	}
	else {
		std::stringstream exceptionMsg;
		exceptionMsg << "Could not understand noise function class: " << noiseFunctionClass
			<< " in JSON object: " << noiseTextureToken << ".\n";
		throw exceptionMsg;
	}
	
	return noiseFunction;
}

std::shared_ptr<MappingLayer> CommonLoaderFunctions::CreateMappingLayer(nlohmann::json const &
																		imageTextureObj) {
	std::string mappingLayerName = SafeGetToken(imageTextureObj, "mapping_layer");
	
	bool hasTransformData = false;
	Matrix4x4 worldToLocal;
	Matrix4x4 localToWorld;
	if (HasKey(imageTextureObj, "mapping_data")) {
		auto mappingData = SafeGetToken(imageTextureObj, "mapping_data");
		if (HasKey(mappingData, "local_to_world_transform")) {
			SetUpTransformFromJsonNode(CommonLoaderFunctions::SafeGetToken(mappingData, "local_to_world_transform"), localToWorld, worldToLocal);
		}
	}
	
	std::shared_ptr<MappingLayer> mappingLayer;
	if (mappingLayerName == "rectangular") {
		auto mappingData = SafeGetToken(imageTextureObj, "mapping_data");
		float recWidth = SafeGetToken(mappingData, "rec_width");
		float recHeight = SafeGetToken(mappingData, "rec_height");
		unsigned int widthAxis = SafeGetToken(mappingData, "width_axis");
		unsigned int heightAxis = SafeGetToken(mappingData, "height_axis");
		float originX = SafeGetToken(mappingData, "origin_x");
		float originY = SafeGetToken(mappingData, "origin_y");
		float originZ = SafeGetToken(mappingData, "origin_z");
		bool useWorldCoordsForTex = false;
		if (HasKey(mappingData, "use_world_coords_for_tex")) {
			useWorldCoordsForTex = SafeGetToken(mappingData, "use_world_coords_for_tex");
		}
		std::string wrapType = SafeGetToken(mappingData, "wrap_type");
		assert(widthAxis < 3);
		assert(heightAxis < 3);
		
		mappingLayer = std::make_shared<RectangularMapping>(recWidth, recHeight,
															widthAxis, heightAxis,
															Point3(originX, originY, originZ),
															GetMappingLayerFromString(wrapType),
															useWorldCoordsForTex);
	}
	else if (mappingLayerName == "spherical") {
		auto mappingData = SafeGetToken(imageTextureObj, "mapping_data");
		float radius = SafeGetToken(mappingData, "radius");
		float originX = SafeGetToken(mappingData, "origin_x");
		float originY = SafeGetToken(mappingData, "origin_y");
		float originZ = SafeGetToken(mappingData, "origin_z");
		std::string wrapType = SafeGetToken(mappingData, "wrap_type");
		bool useWorldCoordsForTex = false;
		if (HasKey(mappingData, "use_world_coords_for_tex")) {
			useWorldCoordsForTex = SafeGetToken(mappingData, "use_world_coords_for_tex");
		}
		mappingLayer = std::make_shared<SphericalMapping>(radius,
														  Vector3(originX, originY, originZ),
														  GetMappingLayerFromString(wrapType),
														  useWorldCoordsForTex);
	}
	else if (mappingLayerName == "model_coord") {
		auto mappingData = SafeGetToken(imageTextureObj, "mapping_data");
		std::string wrapType = SafeGetToken(mappingData, "wrap_type");
		mappingLayer = std::make_shared<ModelCoordMapping>(GetMappingLayerFromString(wrapType));
	}
	else if (mappingLayerName == "null") {
		mappingLayer = std::make_shared<NullMapping>();
	}
	else {
		std::stringstream exceptionMsg;
		exceptionMsg << "Could not understand mapping layer: " << mappingLayerName
			<< " in JSON object: " << imageTextureObj << ".\n";
		throw exceptionMsg;
	}
	mappingLayer->SetInvTransformMatrix(worldToLocal);
	return mappingLayer;
}

MappingLayer::WrapType CommonLoaderFunctions::GetMappingLayerFromString(std::string wrapType) {
	if (wrapType == "clamp") {
		return MappingLayer::WrapType::Clamp;
	}
	else if (wrapType == "repeat") {
		return MappingLayer::WrapType::Repeat;
	}
	else {
		std::stringstream exceptionMsg;
		exceptionMsg << "Could not understand wrap type: " << wrapType
			<< " in JSON object: " << wrapType << ".\n";
		throw exceptionMsg;
	}
}

void CommonLoaderFunctions::SetUpRandomSampler(nlohmann::json const & jsonObj,
											   RandomSamplerType & randomSamplerType,
											   int & numRandomSamples, int & numRandomSets) {
	randomSamplerType = None;
	std::string samplerTypeToken = CommonLoaderFunctions::SafeGetToken(jsonObj, "random_sampler_type");
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
	numRandomSamples = CommonLoaderFunctions::SafeGetToken(jsonObj, "num_random_samples");
	numRandomSets = CommonLoaderFunctions::SafeGetToken(jsonObj, "num_random_sets");
}


Matrix4x4 CommonLoaderFunctions::ConstructMatrixFromJsonNode(
															 nlohmann::json const & matrixNode) {
	auto matrixRow0 = CommonLoaderFunctions::SafeGetToken(matrixNode,
														  "matrix_row_0");
	auto matrixRow1 = CommonLoaderFunctions::SafeGetToken(matrixNode,
														  "matrix_row_1");
	auto matrixRow2 = CommonLoaderFunctions::SafeGetToken(matrixNode,
														  "matrix_row_2");
	auto matrixRow3 = CommonLoaderFunctions::SafeGetToken(matrixNode,
														  "matrix_row_3");
	
	return Matrix4x4(
	(float)matrixRow0[0], (float)matrixRow0[1], (float)matrixRow0[2], (float)matrixRow0[3],
	(float)matrixRow1[0], (float)matrixRow1[1], (float)matrixRow1[2], (float)matrixRow1[3],
	(float)matrixRow2[0], (float)matrixRow2[1], (float)matrixRow2[2], (float)matrixRow2[3],
	(float)matrixRow3[0], (float)matrixRow3[1], (float)matrixRow3[2], (float)matrixRow3[3]);
}

void CommonLoaderFunctions::SetUpTransformFromJsonNode(nlohmann::json const & transformNodeArray, Matrix4x4 & localToWorld, Matrix4x4 & worldToLocal) {
	std::vector<Matrix4x4> localToWorldArray;
	std::vector<Matrix4x4> worldToLocalArray;
	for(auto const & element : transformNodeArray.items()) {
		Matrix4x4 localToWorldElem;
		Matrix4x4 worldToLocalElem;
		auto const &elementValue = element.value();
		
		if (CommonLoaderFunctions::HasKey(elementValue, "scale")) {
			auto const & scaleValue = CommonLoaderFunctions::SafeGetToken(elementValue,
																		  "scale");
			localToWorldElem = Matrix4x4::ScaleMatrix(Vector3((float)scaleValue[0],
															  (float)scaleValue[1],
															  (float)scaleValue[2]));
			worldToLocalElem = Matrix4x4::InvScaleMatrix(Vector3((float)scaleValue[0],
															  (float)scaleValue[1],
															  (float)scaleValue[2]));
		}
		else if (CommonLoaderFunctions::HasKey(elementValue, "translate")) {
			auto const & transVal = CommonLoaderFunctions::SafeGetToken(elementValue,
																		  "translate");
			localToWorldElem = Matrix4x4::TranslationMatrix(Vector3((float)transVal[0],
															  (float)transVal[1],
															  (float)transVal[2]));
			worldToLocalElem = Matrix4x4::InvTranslationMatrix(Vector3((float)transVal[0],
															  (float)transVal[1],
															  (float)transVal[2]));
		}
		else if (CommonLoaderFunctions::HasKey(elementValue, "rotate_x")) {
			float rotValue = CommonLoaderFunctions::SafeGetToken(elementValue,
																		  "rotate_x");
			localToWorldElem = Matrix4x4::RotationMatrixX(rotValue);
			worldToLocalElem = Matrix4x4::InvRotationMatrixX(rotValue);
		}
		else if (CommonLoaderFunctions::HasKey(elementValue, "rotate_y")) {
			float rotValue = CommonLoaderFunctions::SafeGetToken(elementValue,
																		  "rotate_y");
			localToWorldElem = Matrix4x4::RotationMatrixY(rotValue);
			worldToLocalElem = Matrix4x4::InvRotationMatrixY(rotValue);
		}
		else if (CommonLoaderFunctions::HasKey(elementValue, "rotate_z")) {
			float rotValue = CommonLoaderFunctions::SafeGetToken(elementValue,
																		  "rotate_z");
			localToWorldElem = Matrix4x4::RotationMatrixZ(rotValue);
			worldToLocalElem = Matrix4x4::InvRotationMatrixZ(rotValue);
		}
		else {
			std::stringstream exceptionMsg;
			exceptionMsg << "Could not understand transform type: " << elementValue
				<< " in JSON object: " << transformNodeArray << ".\n";
			throw exceptionMsg;
		}
		localToWorldArray.push_back(localToWorldElem);
		worldToLocalArray.push_back(worldToLocalElem);
	}
	
	for(size_t i = 0; i < localToWorldArray.size(); i++) {
		// note the difference in order
		localToWorld = localToWorldArray[i] * localToWorld;
		worldToLocal = worldToLocal * worldToLocalArray[i];
	}
}
