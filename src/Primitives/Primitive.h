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

	virtual bool IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) = 0;
	virtual bool IntersectShadowLocal(const Ray &rayLocal, float tMin, float tMax) = 0;
	
	virtual Vector3 GetNormalWorld(IntersectionResult const &intersectionResult) const = 0;
	
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
	
	virtual void SamplePrimitiveLocal(Point3& resultingSample) = 0;
	virtual void SamplePrimitiveWorld(Point3& resultingSample) = 0;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const {
		return 1.0f;
	}

	const std::string& GetName() const {
		return name;
	}
	
	virtual bool HasBoundingBox() const = 0;
	
	virtual AABBox GetBoundingBoxLocal() const = 0;
	virtual AABBox GetBoundingBoxWorld() const = 0;
	
	bool GetIsTransformed() const {
		return isTransformed;
	}
	
	virtual void SetLocalToWorld(Matrix4x4 const & localToWorld);
	virtual void SetWorldToLocal(Matrix4x4 const & worldToLocal);
	virtual void SetTransformAndInverse(Matrix4x4 const & localToWorld,
								Matrix4x4 const & worldToLocal);
	
	// compound objects might want to accumulate transforms, so
	// make these virtual
	virtual Vector3 GetLocalToWorldDir(Vector3 const & inDir) const;
	virtual Vector3 GetWorldToLocalDir(Vector3 const & inDir) const;
	virtual Vector3 GetLocalToWorldTransposeDir(Vector3 const & inDir) const;
	virtual Vector3 GetWorldToLocalTransposeDir(Vector3 const & inDir) const;
	
	virtual Point3 GetLocalToWorldPos(Point3 const & inPos) const;
	virtual Point3 GetWorldToLocalPos(Point3 const & inPos) const;
	
	virtual Matrix4x4 GetWorldToLocal() const {
		return worldToLocal;
	}
	
	virtual Matrix4x4 GetLocalToWorld() const {
		return localToWorld;
	}
	
	virtual Matrix4x4 GetLocalToWorldTranspose() const {
		return localToWorldTranspose;
	}
	
	virtual Matrix4x4 GetWorldToLocalTranspose() const {
		return worldToLocalTranspose;
	}

protected:
	std::shared_ptr<Material> material;
	std::shared_ptr<GenericSampler> sampler;
	
	Matrix4x4 worldToLocal, localToWorld;
	Matrix4x4 localToWorldTranspose, worldToLocalTranspose;
	bool isTransformed;

	std::string name;
};
