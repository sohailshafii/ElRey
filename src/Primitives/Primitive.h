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
	Primitive(std::shared_ptr<Material> const& iMaterial,
		const std::string& iName)
		: material(iMaterial), name(iName), isTransformed(false)
	{ }
	Primitive(std::shared_ptr<Material> const& iMaterial,
			std::shared_ptr<GenericSampler> const& iSampler,
			const std::string& iName)
		: material(iMaterial), sampler(iSampler), name(iName), isTransformed(false)
	{ }

	Primitive(std::shared_ptr<Material> && material,
		const std::string& iName)
		: material(std::move(material)), name(iName), isTransformed(false)
	{ }
	Primitive(std::shared_ptr<Material> && material,
		std::shared_ptr<GenericSampler> && sampler,
		const std::string& iName)
		: material(std::move(material)),
			sampler(std::move(sampler)), name(iName), isTransformed(false)
	{ }

	virtual ~Primitive() { }

	virtual bool Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) = 0;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) = 0;
	virtual Vector3 GetNormalAtPosition(IntersectionResult const &intersectionResult) const = 0;
	
	// a compound object might have a different material per sub-object
	virtual std::shared_ptr<Material> GetMaterial() {
		return material;
	}
	
	void SetSampler(std::shared_ptr<GenericSampler> sampler) {
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
	
	bool GetIsTransformed() const {
		return isTransformed;
	}
	
	void SetLocalToWorld(Matrix4x4 const & localToWorld);
	void SetWorldToLocal(Matrix4x4 const & worldToLocal);
	void SetTransformAndInverse(Matrix4x4 const & localToWorld,
								Matrix4x4 const & worldToLocal);
	
	// compound objects might want to accumulate transforms, so
	// make these virtual
	virtual Vector3 GetLocalToWorldDir(Vector3 const & inDir) const;
	virtual Vector3 GetWorldToLocalDir(Vector3 const & inDir) const;
	virtual Vector3 GetWorldToLocalTransposeDir(Vector3 const & inDir) const;
	
	virtual Point3 GetLocalToWorldPos(Point3 const & inPos) const;
	virtual Point3 GetWorldToLocalPos(Point3 const & inPos) const;
	virtual Point3 GetWorldToLocalTransposePos(Point3 const & inPos) const;
	
	Matrix4x4 const & GetWorldToLocal() const {
		return worldToLocal;
	}
	
	Matrix4x4 const & GetLocalToWorld() const {
		return localToWorld;
	}
	
	Matrix4x4 const & GetLocalToWorldTranspose() const {
		return worldToLocalTranspose;
	}

protected:
	std::shared_ptr<Material> material;
	std::shared_ptr<GenericSampler> sampler;
	
	Matrix4x4 worldToLocal, localToWorld, worldToLocalTranspose;
	bool isTransformed;

	std::string name;
};
