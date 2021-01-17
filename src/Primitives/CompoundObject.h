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
		for(Primitive* prim : primitives) {
			delete prim;
		}
		primitives.clear();
	}
	
	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
								 IntersectionResult &intersectionResult) override;
	virtual Primitive* IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(const ShadingInfo& shadingInfo) const override;
	
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
	
	void AddPrimitive(Primitive * primitive);
	void RemovePrimitiveAtIndex(unsigned int index);
	void RemovePrimitiveWithName(std::string const & name);
	
	void PrintBounds();
	
	virtual Primitive* GetSubPrimitiveByName(std::string const & intersecPrimName) const override;
	
private:
	std::vector<Primitive*> primitives;
	AABBox boundingBox;
	
	void RecomputeBoundingBox();
};

