#pragma once

#include "Primitive.h"
#include <vector>
#include <string>

class CompoundObject : public Primitive {
public:
	CompoundObject(std::shared_ptr<Material> & iMaterial,
			const std::string& iName,
			std::vector<std::string> const & childrenNames) :
		Primitive(iMaterial, iName), childrenNames(childrenNames) {
	}

	CompoundObject(std::shared_ptr<Material> && iMaterial,
		const std::string& iName,
		std::vector<std::string> const & childrenNames) :
		Primitive(iMaterial, iName), childrenNames(childrenNames) {
	}

	bool Intersect(const Ray &ray, float tMin, float& tMax,
					IntersectionResult &intersectionResult) override;
	bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalAtPosition(IntersectionResult const &intersectionResult) const override;
	
	virtual void SamplePrimitive(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override {
		return boundingBox;
	}
	
	std::vector<std::string> const & GetChildrenNames() const {
		return childrenNames;
	}
	
	void AddPrimitive(Primitive * primitive);
	void RemovePrimitiveAtIndex(unsigned int index);
	void RemovePrimitiveWithName(std::string const & name);

private:
	AABBox boundingBox;
	std::vector<Primitive*> primitives;
	Primitive *closestPrimSoFar;
	std::vector<std::string> childrenNames;
	
	Primitive* GetPrimitiveByIntersectionResult(IntersectionResult const &intersectionResult) const;
};

