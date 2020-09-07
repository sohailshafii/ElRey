#pragma once

#include "Primitive.h"

class InstancePrimitive : public Primitive {
public:
	InstancePrimitive(std::string const & iName,
					  // don't own
					  Primitive* primitive);
	
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
	
	virtual void SamplePrimitiveLocal(Point3& resultingSample) override;
	virtual void SamplePrimitiveWorld(Point3& resultingSample) override;
	virtual bool HasBoundingBox() const override;
	
	virtual AABBox GetBoundingBoxLocal() const override;
	virtual AABBox GetBoundingBoxWorld() const override;
	
	virtual bool Intersect(const Ray &rayWorld, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadow(const Ray &rayWorld, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(IntersectionResult const &intersectionResult)
		const override;
	
protected:
	virtual bool IntersectLocal(const Ray &rayLocal, float tMin,
								float& tMax,
								IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadowLocal(const Ray &rayLocal,
									  float tMin, float tMax) override;
	virtual Vector3 GetNormalLocal(IntersectionResult const
								   &intersectionResult) const override;
	
private:
	// don't own
	Primitive* instancePrimitive;
};
