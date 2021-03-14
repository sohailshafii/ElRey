#pragma once

#include <memory>
#include <string>
#include "Materials/Material.h"
#include "SceneData/Light.h"
#include "Primitives/Primitive.h"

// note that the primitive we store here is also traced against
// we just need to make sure that we when we intersect said
// primitive we don't trace against its own light.
class AreaLight : public Light
{
public:
	AreaLight(bool castsShadows, std::string primitiveName);
	AreaLight(bool castsShadows,
		Primitive* primitive);
	~AreaLight();
	
	virtual void SetPrimitive(std::shared_ptr<Primitive> newPrimitive) override {
		primitive = newPrimitive;
	}
	
	virtual std::string GetPrimitiveName() const override {
		if (primitive != nullptr) {
			return primitive->GetName();
		}
		return primitiveName;
	}
	
	virtual Vector3 GetDirectionFromPositionScaled(const ShadingInfo& shadingInfo) const override;
	virtual Color3 GetRadiance(ShadingInfo& shadingInfo,
							   const Scene& scene) const override;
	
	virtual bool IsAmbient() const override {
		return false;
	}

	virtual const Primitive* GetPrimitive() const override {
		return primitive.get();
	}

	virtual const bool IsAreaLight() const override {
		return true;
	}

	virtual void ModifyShadingInfoForAreaLight(ShadingInfo& shadingInfo) const override;

	virtual float GeometricTerm(const ShadingInfo& shadingInfo)
		const override;

	virtual float PDF(const ShadingInfo& shadingInfo) const override {
		return primitive->PDF(shadingInfo);
	}

private:
	std::shared_ptr<Primitive> primitive;
	std::string primitiveName;
};
