#pragma once

#include "Math/Vector3.h"
#include "Math/Point3.h"
#include "Materials/Color3.h"
#include "IntersectionResult.h"
#include "Primitives/Primitive.h"
#include <string>

class Scene;

class Light {
public:
	Light(bool castsShadows)
		: castsShadows(castsShadows) {
		
	}
	virtual ~Light() {
		
	}
	
	virtual void SetPrimitive(Primitive* newPrimitive) {
		// must override
	}
	
	virtual std::string GetPrimitiveName() const {
		return " ";
	}
	
	virtual Color3 GetRadiance(const IntersectionResult& intersectionRes, const Scene& scene) = 0;
	
	bool CastsShadows() {
		return castsShadows;
	}
	
	virtual bool IsAmbient() const {
		return false;
	}
	
	virtual bool IsLightDistanceInfinite() const {
		return false;
	}

	virtual float GeometricTerm(
		const IntersectionResult& intersectionRes) const {
		return 1.0f;
	}

	virtual float PDF(
		const IntersectionResult& intersectionRes) const {
		return 1.0f;
	}

	virtual const bool IsAreaLight() const {
		return false;
	}

	virtual const Primitive* GetPrimitive() const {
		return nullptr;
	}

	virtual void ComputeAndStoreAreaLightInformation(
		IntersectionResult& intersectionRes) const
	{
		// for area lights
	}

	virtual Vector3 GetDirectionFromPosition(
		const IntersectionResult& intersectionRes) const = 0;
	
private:
	bool castsShadows;
};
