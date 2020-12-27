#pragma once

#include <memory>
#include "Materials/Color.h"
#include "Math/Ray.h"
#include "Math/Matrix4x4.h"
#include "Materials/Material.h"
#include "Math/AABBox.h"
#include "SceneData/IntersectionResult.h"
#include "SceneData/ParamsForNormal.h"
#include "Sampling/GenericSampler.h"
#include <string>

class Primitive {
public:
	Primitive(std::string const & iName) : name(iName),
		usedForInstancing(false) {
	}
	
	Primitive(std::shared_ptr<Material> const& iMaterial,
			  const std::string& iName)
		: material(iMaterial), name(iName), usedForInstancing(false)
	{ }
	Primitive(std::shared_ptr<Material> const& iMaterial,
			  std::shared_ptr<GenericSampler> const& iSampler,
			  const std::string& iName)
		: material(iMaterial), sampler(iSampler), name(iName),
			usedForInstancing(false)
	{ }

	virtual ~Primitive() { }

	// TODO: make it obvious that the last two arguments are modified upon succesful intersection
	virtual bool Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) = 0;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) = 0;
	
	virtual Vector3 ComputeHardNormal(Point3 const & position) const = 0;
	virtual Vector3 GetNormal(ParamsForNormal const &paramsForNormal) const = 0;
	
	// a compound object might have a different material per sub-object
	// which is why intersection result is required
	virtual Material const * GetMaterial(IntersectionResult const & intersectionResult) {
		return material.get();
	}
	
	virtual void SetSampler(std::shared_ptr<GenericSampler> const & sampler) {
		this->sampler = sampler;
	}
	
	// a compound object might have a different sampler per-subbject
	virtual GenericSampler const * GetSampler(IntersectionResult const & intersectionResult) {
		return sampler.get();
	}
	
	virtual void SamplePrimitive(Point3& resultingSample,
								 IntersectionResult const & intersectionResult) = 0;
	
	virtual float PDF(ParamsForNormal const &paramsForNormal) const {
		return 1.0f;
	}

	const std::string& GetName() const {
		return name;
	}
	
	virtual bool HasBoundingBox() const = 0;
	
	virtual AABBox GetBoundingBox() const = 0;
	
	bool UsedForInstancing() const {
		return usedForInstancing;
	}
	
	void SetUsedForInstancing(bool setForInstancing) {
		usedForInstancing = setForInstancing;
	}

	virtual Primitive* GetSubPrimitiveByName(std::string const & intersecPrimName) const {
		return nullptr;
	}
	
protected:
	std::shared_ptr<Material> material;
	std::shared_ptr<GenericSampler> sampler;

	std::string name;
	bool usedForInstancing;
};
