#pragma once

#include "Materials/Color.h"
#include "Materials/Color3.h"
#include "SceneData/ShadingInfo.h"

class Material {
public:
	Material() {
		
	}
	virtual ~Material() {
		
	}
	
	struct DirectionSample {
		DirectionSample(Color const & color, float pdf,
						Vector3 const & wi) {
			this->color = color;
			this->pdf = pdf;
			this->wi = wi;
		}
		
		Color color;
		float pdf;
		Vector3 wi;
	};
	
	struct SecondaryVectorInfo {
		SecondaryVectorInfo(Vector3 const & vector, float coeff,
							Color3 color = Color3::Black(),
							Color3 colorFilter = Color3::Black(),
							bool useColorFilter = false) {
			this->direction = vector;
			this->vecCoeff = coeff;
			this->colorComp = color;
			
			this->colorFilter = colorFilter;
			this->useColorFilter = useColorFilter;
		}
		
		Vector3 direction;
		float vecCoeff;
		Color3 colorComp;
		
		Color3 colorFilter;
		bool useColorFilter;
	};
	
	virtual Color GetAmbientColor(const ShadingInfo &shadingInfo) const = 0;
	virtual Color GetDirectColor(ShadingInfo const  &shadingInfo) const = 0;
	virtual void SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<DirectionSample> & directionSamples) const = 0;
	
	virtual void SetSampler(class GenericSampler *sampler) = 0;
	
	virtual void GetSecondaryVectors(ShadingInfo const & shadingInfo, 							std::vector<SecondaryVectorInfo> & secondaryVectors) const {
	}
};
