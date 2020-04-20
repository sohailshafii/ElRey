#pragma once

#include <memory>
#include "Materials/Color.h"
#include "Math/Ray.h"
#include "Materials/Material.h"
#include "SceneData/IntersectionResult.h"
#include "Sampling/GenericSampler.h"

class Primitive {
public:
	// TODO: by ref or not?
	Primitive(std::shared_ptr<Material> material)
		: material(material)
	{ }
	Primitive(std::shared_ptr<Material> material,
			  std::shared_ptr<GenericSampler> sampler)
		: material(material), sampler(sampler)
	{ }
	virtual ~Primitive() { }

	virtual bool Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) = 0;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) = 0;
	virtual Vector3 GetNormalAtPosition(const Point3& position) const = 0;
	
	std::shared_ptr<Material> GetMaterial() {
		return material;
	}
	
	void SetSampler(std::shared_ptr<GenericSampler> sampler) {
		this->sampler = sampler;
	}
	
	const GenericSampler* GetSampler() {
		return sampler.get();
	}
	
	virtual void SamplePrimitive(Point3& resultingSample) = 0;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const {
		return 1.0f;
	}

protected:
	std::shared_ptr<Material> material;
	std::shared_ptr<GenericSampler> sampler;
};
