#pragma once

#include "Primitive.h"
#include <vector>
#include <string>

class CompoundObject : public Primitive {
public:
	CompoundObject(std::shared_ptr<Material> & iMaterial,
			const std::string& iName) :
		Primitive(iMaterial, iName) {
	}
	
	~CompoundObject() {
		primitives.clear();
	}
	
	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
								 IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Point3 ComputeLocalIntersectionPoint(ShadingInfo const & shadingInfo,
												 Point3 const & intersectionPoint) override;
	
	virtual Vector3 GetNormal(ShadingInfo const & shadingInfo) const override;
	
	virtual Material const * GetMaterial(const ShadingInfo& shadingInfo) override;
	
	virtual Vector3 ComputeHardNormal(Point3 const &position) const override;
	
	virtual void SamplePrimitive(Point3& resultingSample,
								 const ShadingInfo& shadingInfo) override;
	
	// a compound object might have a different sampler per-subbject
	virtual const GenericSampler* GetSampler(const ShadingInfo& shadingInfo) override;
	
	virtual float PDF(const ShadingInfo& shadingInfo) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override;
	
	void AddPrimitive(std::shared_ptr<Primitive> primitive);
	void RemovePrimitiveAtIndex(unsigned int index);
	void RemovePrimitiveWithName(std::string const & name);
	
	void PrintBounds();
	
	virtual std::shared_ptr<Primitive> GetSubPrimitiveByName(std::string const & intersecPrimName) const override;
	
private:
	std::vector<std::shared_ptr<Primitive>> primitives;
	AABBox boundingBox;
	
	void RecomputeBoundingBox();
};

