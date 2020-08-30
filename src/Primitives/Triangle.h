#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"
#include <iostream>

class Triangle : public Primitive {
public:
	Triangle(Point3 const & p0, Point3 const & p1,
		   Point3 const & p2,
		   std::shared_ptr<Material> const& iMaterial,
		   const std::string& iName) : Primitive(iMaterial, iName),
		p0(p0), p1(p1), p2(p2) {
		normalVec = (p1-p0)^(p2-p0);
		normalVec.Normalize();
		ComputeLocalBoundingBox();
	}

	Triangle(Point3 const & p0, Point3 const & p1,
		   Point3 const & p2,
		   std::shared_ptr<Material> && iMaterial,
		   const std::string& iName) : Primitive(iMaterial, iName),
		p0(p0), p1(p1), p2(p2) {
		normalVec = (p2-p0)^(p1-p0);
		normalVec.Normalize();
		ComputeLocalBoundingBox();
	}

	bool IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
				   IntersectionResult &intersectionResult) override;
	bool IntersectShadowLocal(const Ray &rayLocal, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalWorld(IntersectionResult const &intersectionResult) const override {
		return isTransformed ? GetWorldToLocalTransposeDir(normalVec).Normalized() : normalVec;
	}
	
	virtual void SamplePrimitiveLocal(Point3& resultingSample) override;
	
	virtual void SamplePrimitiveWorld(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBoxLocal() const override;
	
	virtual AABBox GetBoundingBoxWorld() const override;

private:
	Point3 p0, p1, p2;
	Vector3 normalVec;
	AABBox boundingBox;
	
	AABBox ComputeLocalBoundingBox() const;
};
