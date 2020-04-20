#pragma once

#include <memory>
#include "Materials/Material.h"
#include "SceneData/Light.h"
#include "Primitives/Primitive.h"

// note that the primitive we store here is also traced against
// we just need to make sure that we when we intersect said
// primitive we don't trace against its own light.
class AreaLight : public Light
{
public:
	AreaLight(std::shared_ptr<Primitive> primitive);
	~AreaLight();
	
	virtual Vector3 GetDirectionFromPosition(
		const IntersectionResult& intersectionRes) const override;
	virtual Color3 GetRadiance(const IntersectionResult& intersectionRes, const Scene& scene) override;
	
	virtual bool IsAmbient() const override {
		return true;
	}

	virtual const Primitive* GetPrimitive() const override {
		return primitive.get();
	}

	virtual const bool NeedsToBeSampled() const {
		return false;
	}

	virtual void StoreAreaLightInformation(
		IntersectionResult& intersectionRes) const override;

	virtual float GeometricTerm(
		const IntersectionResult& intersectionRes)
		const override;

	virtual float PDF(
		const IntersectionResult& intersectionRes) const override {
		return primitive->PDF(intersectionRes);
	}

private:
	std::shared_ptr<Primitive> primitive;
};
