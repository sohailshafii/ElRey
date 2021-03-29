#pragma once

#include "Materials/Color.h"
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
							Color color = Color::Black()) {
			direction = vector;
			vecCoeff = coeff;
			colorComp = color;
		}
		
		Vector3 direction;
		float vecCoeff;
		Color colorComp;
	};
	
	virtual Color GetAmbientColor(const ShadingInfo &shadingInfo) const = 0;
	virtual Color GetDirectColor(ShadingInfo const  &shadingInfo) const = 0;
	virtual void SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<DirectionSample> & directionSamples) const = 0;
	
	virtual void SetSampler(class GenericSampler *sampler) = 0;
	
	virtual void GetSecondaryVectors(ShadingInfo const & shadingInfo, 							std::vector<SecondaryVectorInfo> & secondaryVectors) const {
	}
};
