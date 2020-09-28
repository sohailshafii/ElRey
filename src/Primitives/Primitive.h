#pragma once

#include <memory>
#include "Materials/Color.h"
#include "Math/Ray.h"
#include "Math/Matrix4x4.h"
#include "Materials/Material.h"
#include "Math/AABBox.h"
#include "SceneData/IntersectionResult.h"
#include "Sampling/GenericSampler.h"
#include <string>

class Primitive {
public:
	Primitive(std::string const & iName) : name(iName) {
	}
	
	Primitive(std::shared_ptr<Material> const& iMaterial,
			  const std::string& iName)
		: material(iMaterial), name(iName)
	{ }
	Primitive(std::shared_ptr<Material> const& iMaterial,
			  std::shared_ptr<GenericSampler> const& iSampler,
			  const std::string& iName)
		: material(iMaterial), sampler(iSampler), name(iName)
	{ }

	virtual ~Primitive() { }

	virtual bool Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) = 0;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) = 0;
	
	virtual Vector3 GetNormal(IntersectionResult const &intersectionResult) const = 0;
	
	// a compound object might have a different material per sub-object
	virtual std::shared_ptr<Material> GetMaterial() {
		return material;
	}
	
	virtual void SetSampler(std::shared_ptr<GenericSampler> sampler) {
		this->sampler = sampler;
	}
	
	// a compound object might have a different sampler per-subbject
	virtual const GenericSampler* GetSampler() {
		return sampler.get();
	}
	
	virtual void SamplePrimitive(Point3& resultingSample) = 0;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const {
		return 1.0f;
	}

	const std::string& GetName() const {
		return name;
	}
	
	virtual bool HasBoundingBox() const = 0;
	
	virtual AABBox GetBoundingBox() const = 0;

protected:
	std::shared_ptr<Material> material;
	std::shared_ptr<GenericSampler> sampler;

	std::string name;
};
