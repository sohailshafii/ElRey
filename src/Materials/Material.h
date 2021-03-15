#pragma once

#include "Materials/Color.h"
#include "SceneData/ShadingInfo.h"

class Material {
public:
	Material() {
		
	}
	virtual ~Material() {
		
	}
	
	virtual Color GetAmbientColor(const ShadingInfo &shadingInfo) const = 0;
	virtual Color GetDirectColor(ShadingInfo &shadingInfo) const = 0;
	virtual Color SampleColorAndDirections(ShadingInfo &shadingInfo, std::vector<float>& pdfs, std::vector<Vector3> &wis) const = 0;
	
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
