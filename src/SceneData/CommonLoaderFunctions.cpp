#include "SceneData/CommonLoaderFunctions.h"
#include "Materials/LambertianMaterial.h"
#include "Materials/PhongMaterial.h"
#include "Materials/SimpleEmissiveMaterial.h"
#include "Materials/ReflectiveMaterial.h"
#include "Materials/GlossySpecularMaterial.h"
#include "Materials/TransparentMaterial.h"
#include "Materials/DielectricMaterial.h"
#include "Materials/Texturing/SingleColorTex.h"
#include "Materials/Texturing/NullMapping.h"
#include <sstream>
#include <vector>
#include <iostream>

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

std::shared_ptr<Material> CommonLoaderFunctions::CreateMaterial(
																nlohmann::json const & jsonObj) {
	std::shared_ptr<Material> newMaterial;

	std::string primitiveType = SafeGetToken(jsonObj, "type");
	
	if (primitiveType == "lambertian") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		auto colorObj = SafeGetToken(jsonObj, "color");
		
		newMaterial = std::make_shared<LambertianMaterial>(kA, kD,
			std::make_shared<SingleColorTex>(std::make_shared<NullMapping>(),
											 Color3(colorObj[0], colorObj[1], colorObj[2])));
	}
	else if (primitiveType == "phong") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		float kS = SafeGetToken(jsonObj, "ks");
		float exponent = SafeGetToken(jsonObj, "exponent");
		auto colorObj = SafeGetToken(jsonObj, "color");
		auto ksColor = SafeGetToken(jsonObj, "ks_color");
		newMaterial = std::make_shared<PhongMaterial>(kA, kD, kS, exponent,
			std::make_shared<SingleColorTex>(std::make_shared<NullMapping>(),
											 Color3(colorObj[0], colorObj[1], colorObj[2])),
			std::make_shared<SingleColorTex>(std::make_shared<NullMapping>(),
											 Color3(ksColor[0], ksColor[1], ksColor[2])));
	}
	else if (primitiveType == "reflective") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		float kS = SafeGetToken(jsonObj, "ks");
		float cR = SafeGetToken(jsonObj, "cr");
		float kR = SafeGetToken(jsonObj, "kr");
		float exponent = SafeGetToken(jsonObj, "exponent");
		auto colorObj = SafeGetToken(jsonObj, "color");
		auto ksColor = SafeGetToken(jsonObj, "ks_color");
		newMaterial = std::make_shared<ReflectiveMaterial>(kA, kD, kS, exponent,
			std::make_shared<SingleColorTex>(std::make_shared<NullMapping>(),
											 Color3(colorObj[0], colorObj[1], colorObj[2])),
			std::make_shared<SingleColorTex>(std::make_shared<NullMapping>(),
											 Color3(ksColor[0], ksColor[1], ksColor[2])), cR, kR);
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
		auto colorObj = SafeGetToken(jsonObj, "color");
		auto ksColor = SafeGetToken(jsonObj, "ks_color");
		newMaterial = std::make_shared<TransparentMaterial>(kA, kD, kS, exponent,
			std::make_shared<SingleColorTex>(std::make_shared<NullMapping>(),
											 Color3(colorObj[0], colorObj[1], colorObj[2])),
			std::make_shared<SingleColorTex>(std::make_shared<NullMapping>(),
											 Color3(ksColor[0], ksColor[1], ksColor[2])), cR, kR,
			eta, kt);
	}
	else if (primitiveType == "dielectric") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		float kS = SafeGetToken(jsonObj, "ks");
		float etaIn = SafeGetToken(jsonObj, "eta_in");
		float etaOut = SafeGetToken(jsonObj, "eta_out");
		float exponent = SafeGetToken(jsonObj, "exponent");
		auto colorObj = SafeGetToken(jsonObj, "color");
		auto ksColor = SafeGetToken(jsonObj, "ks_color");
		
		auto cfIn = SafeGetToken(jsonObj, "cf_in");
		auto cfOut = SafeGetToken(jsonObj, "cf_out");
		newMaterial = std::make_shared<DielectricMaterial>(kA, kD, kS, exponent,
			std::make_shared<SingleColorTex>(std::make_shared<NullMapping>(),
											 Color3(colorObj[0], colorObj[1], colorObj[2])),
			std::make_shared<SingleColorTex>(std::make_shared<NullMapping>(),
											 Color3(ksColor[0], ksColor[1], ksColor[2])),
			etaIn, etaOut, Color3(cfIn[0], cfIn[1], cfIn[2]),
			Color3(cfOut[0], cfOut[1], cfOut[2]));
	}
	else if (primitiveType == "simple_emissive") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		auto colorObj = SafeGetToken(jsonObj, "color");
		newMaterial = std::make_shared<SimpleEmissiveMaterial>
			(kA, kD, std::make_shared<SingleColorTex>(std::make_shared<NullMapping>(),
													  Color3(colorObj[0], colorObj[1], colorObj[2])));
	}
	else if (primitiveType == "glossy_specular") {
		float kA = SafeGetToken(jsonObj, "ka");
		float kD = SafeGetToken(jsonObj, "kd");
		float kS = SafeGetToken(jsonObj, "ks");
		float cR = SafeGetToken(jsonObj, "cr");
		float kR = SafeGetToken(jsonObj, "kr");
		float exponent = SafeGetToken(jsonObj, "exponent");
		auto colorObj = SafeGetToken(jsonObj, "color");
		auto ksColor = SafeGetToken(jsonObj, "ks_color");
		newMaterial = std::make_shared<GlossySpecularMaterial>(kA, kD, kS,
			exponent, std::make_shared<SingleColorTex>(std::make_shared<NullMapping>(),
													   Color3(colorObj[0], colorObj[1], colorObj[2])),
			std::make_shared<SingleColorTex>(std::make_shared<NullMapping>(),
											 Color3(ksColor[0], ksColor[1], ksColor[2])), cR, kR);
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
