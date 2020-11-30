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
	
	virtual bool Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(ParamsForNormal const &paramsForNormal) const override;
	
	//virtual Vector3 GetNormalAtPosition(Point3 const &position) const override;
	
	virtual void SamplePrimitive(Point3& resultingSample,
								 IntersectionResult const & intersectionResult) override;
		
	virtual float PDF(ParamsForNormal const &paramsForNormal) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override;
	
	virtual Material const * GetMaterial(IntersectionResult const & intersectionResult) override;
	virtual const GenericSampler* GetSampler(IntersectionResult const & intersectionResult) override;
	
	void AddPrimitive(Primitive * primitive);
	void RemovePrimitiveAtIndex(unsigned int index);
	void RemovePrimitiveWithName(std::string const & name);
	
	void PrintBounds();
	
private:
	std::vector<Primitive*> primitives;
	AABBox boundingBox;
	
	Primitive* GetPrimitiveByName(std::string const & intersecPrimName) const;
	void RecomputeBoundingBox();
};

