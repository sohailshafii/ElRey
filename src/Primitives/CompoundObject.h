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
	
	virtual bool Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(ParamsForNormal const &paramsForNormal) const override;
	
	virtual Vector3 GetNormalAtPosition(Point3 const &position) const override;
	
	virtual void SamplePrimitive(Point3& resultingSample) override;
		
	virtual float PDF(ParamsForNormal const &paramsForNormal) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override;
	
	virtual Material const * GetMaterial() override;
	virtual const GenericSampler* GetSampler() override;
	
	void AddPrimitive(Primitive * primitive);
	void RemovePrimitiveAtIndex(unsigned int index);
	void RemovePrimitiveWithName(std::string const & name);
	
private:
	std::vector<Primitive*> primitives;
	Primitive *closestPrimSoFar;
	AABBox boundingBox;
	
	Primitive* GetPrimitiveByParams(ParamsForNormal const &paramsForNormal) const;
	void RecomputeBoundingBox();
};

