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
	
	virtual void SetLocalToWorld(Matrix4x4 const & localToWorld);
	virtual void SetWorldToLocal(Matrix4x4 const & worldToLocal);
	virtual void SetTransformAndInverse(Matrix4x4 const & localToWorld,
										Matrix4x4 const & worldToLocal);
	
private:
	// don't own
	Primitive* instancePrimitive;
	
	Matrix4x4 worldToLocal, localToWorld;
	Matrix4x4 localToWorldTranspose, worldToLocalTranspose;
};
