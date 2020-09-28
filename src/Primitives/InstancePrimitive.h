#pragma once

#include "Primitive.h"

class InstancePrimitive : public Primitive {
public:
	InstancePrimitive(std::string const & iName,
					  // don't own
					  Primitive* primitive);
	
	virtual bool Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(IntersectionResult const &intersectionResult) const override;
	
	virtual std::shared_ptr<Material> GetMaterial() override {
		return instancePrimitive->GetMaterial();
	}
	
	virtual void SetSampler(std::shared_ptr<GenericSampler> sampler) override {
		instancePrimitive->SetSampler(sampler);
	}
	
	// a compound object might have a different sampler per-subbject
	virtual const GenericSampler* GetSampler() override {
		return instancePrimitive->GetSampler();
	}
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override {
		return instancePrimitive->PDF(intersectionResult);
	}
	
	virtual void SamplePrimitive(Point3& resultingSample) override;
	virtual bool HasBoundingBox() const override;
	
	virtual AABBox GetBoundingBox() const override;
	
	void SetLocalToWorld(Matrix4x4 const & localToWorld);
	void SetWorldToLocal(Matrix4x4 const & worldToLocal);
	void SetTransformAndInverse(Matrix4x4 const & localToWorld,
								Matrix4x4 const & worldToLocal);
	
	Vector3 GetLocalToWorldDir(Vector3 const & inDir) const;
	Vector3 GetWorldToLocalDir(Vector3 const & inDir) const;
	Vector3 GetLocalToWorldTransposeDir(Vector3 const & inDir) const;
	Vector3 GetWorldToLocalTransposeDir(Vector3 const & inDir) const;
	
	Point3 GetLocalToWorldPos(Point3 const & inPos) const;
	Point3 GetWorldToLocalPos(Point3 const & inPos) const;
	
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
	
private:
	// don't own
	Primitive* instancePrimitive;
	
	Matrix4x4 worldToLocal, localToWorld;
	Matrix4x4 localToWorldTranspose, worldToLocalTranspose;
};
