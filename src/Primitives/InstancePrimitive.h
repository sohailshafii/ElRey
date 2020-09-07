#pragma once

#include "Primitive.h"

class InstancePrimitive : public Primitive {
public:
	InstancePrimitive(std::shared_ptr<Material> const& iMaterial,
					  const std::string& iName,
					  std::shared_ptr<Primitive> & primitive);
	InstancePrimitive(std::shared_ptr<Material> const& iMaterial,
					  std::shared_ptr<GenericSampler> const& iSampler,
					  const std::string& iName,
					  std::shared_ptr<Primitive> & primitive);
	
	InstancePrimitive(std::shared_ptr<Material> && material,
					  const std::string& iName,
					  std::shared_ptr<Primitive> & primitive);
	InstancePrimitive(std::shared_ptr<Material> && material,
					  std::shared_ptr<GenericSampler> && sampler,
					  const std::string& iName,
					  std::shared_ptr<Primitive> & primitive);
	
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
	
	// TODO
protected:
	virtual bool IntersectLocal(const Ray &rayLocal, float tMin,
								float& tMax,
								IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadowLocal(const Ray &rayLocal,
									  float tMin, float tMax) override;
	virtual Vector3 GetNormalLocal(IntersectionResult const
								   &intersectionResult) const override;
	
private:
	std::shared_ptr<Primitive> instancePrimitive;
};
