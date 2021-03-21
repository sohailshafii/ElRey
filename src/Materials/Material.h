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
	
	virtual Color GetAmbientColor(const ShadingInfo &shadingInfo) const = 0;
	virtual Color GetDirectColor(ShadingInfo &shadingInfo) const = 0;
	virtual void SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<DirectionSample> & directionSamples) const = 0;
	
	virtual void SetSampler(class GenericSampler *sampler) = 0;
	
	virtual bool DoesSurfaceReflect() const {
		return false;
	}
	virtual Vector3 ReflectVectorOffSurface(Vector3 const &normal,
											Vector3 const &incomingVecFacingAwaySurface) const;
	virtual float GetReflectivity() const {
		return 0.0f;
	}
};
