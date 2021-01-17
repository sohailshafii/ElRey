#pragma once

#include <memory>
#include "Materials/Color.h"
#include "Math/Ray.h"
#include "Math/Matrix4x4.h"
#include "Materials/Material.h"
#include "Math/AABBox.h"
#include "SceneData/IntersectionResult.h"
#include "SceneData/ShadingInfo.h"
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

	// TODO: make it obvious that the last two arguments are modified upon succesful intersection
	// if intersection is succesful, return primitive hit. this is useful for compound objects,
	// which might have multiple objects inside of them
	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
								 IntersectionResult &intersectionResult) = 0;
	virtual Primitive* IntersectShadow(const Ray &ray, float tMin, float tMax) = 0;
	
	virtual Vector3 ComputeHardNormal(Point3 const & position) const = 0;
	virtual Vector3 GetNormal(const ShadingInfo &shadingInfo) const = 0;
	
	// a compound object might have a different material per sub-object
	// which is why intersection result is required
	virtual Material const * GetMaterial(const ShadingInfo& shadingInfo) {
		return material.get();
	}
	
	virtual void SetSampler(std::shared_ptr<GenericSampler> const & sampler) {
		this->sampler = sampler;
	}
	
	// a compound object might have a different sampler per-subbject
	virtual GenericSampler const * GetSampler(const ShadingInfo& shadingInfo) {
		return sampler.get();
	}
	
	virtual void SamplePrimitive(Point3& resultingSample,
								 const ShadingInfo &shadingInfo) = 0;
	
	virtual float PDF(const ShadingInfo &shadingInfo) const {
		return 1.0f;
	}

	const std::string& GetName() const {
		return name;
	}
	
	virtual bool HasBoundingBox() const = 0;
	
	virtual AABBox GetBoundingBox() const = 0;

	virtual Primitive* GetSubPrimitiveByName(std::string const & intersecPrimName) const {
		return nullptr;
	}
	
protected:
	std::shared_ptr<Material> material;
	std::shared_ptr<GenericSampler> sampler;

	std::string name;
};
