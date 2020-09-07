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

	CompoundObject(std::shared_ptr<Material> && iMaterial,
		const std::string& iName) :
		Primitive(iMaterial, iName) {
	}
	
	virtual void SamplePrimitiveLocal(Point3& resultingSample) override;
	virtual void SamplePrimitiveWorld(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;
	
	virtual bool HasBoundingBox() const override {
		return false;
	}
	
	virtual AABBox GetBoundingBoxLocal() const override;
	
	virtual AABBox GetBoundingBoxWorld() const override;
	
	virtual std::shared_ptr<Material> GetMaterial() override;
	virtual const GenericSampler* GetSampler() override;
	
	void AddPrimitive(Primitive * primitive);
	void RemovePrimitiveAtIndex(unsigned int index);
	void RemovePrimitiveWithName(std::string const & name);

protected:
	virtual bool IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
								IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadowLocal(const Ray &rayLocal, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalLocal(IntersectionResult const &intersectionResult) const override;
	
private:
	std::vector<Primitive*> primitives;
	Primitive *closestPrimSoFar;
	AABBox localBoundingBox;
	
	Primitive* GetPrimitiveByIntersectionResult(IntersectionResult const &intersectionResult) const;
	void RecomputeBoundingBox();
};

