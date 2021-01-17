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
		ComputeBoundingBox();
	}

	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
								 IntersectionResult &intersectionResult) override;
	virtual Primitive* IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(const ShadingInfo &shadingInfo) const override {
		return normalVec;
	}
	
	virtual Vector3 ComputeHardNormal(Point3 const &position) const override {
		return normalVec;
	}
	
	virtual void SamplePrimitive(Point3& resultingSample,
								 const ShadingInfo &shadingInfo) override;
	
	virtual float PDF(const ShadingInfo &shadingInfo) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override;

private:
	Point3 p0, p1, p2;
	Vector3 normalVec;
	AABBox boundingBox;
	
	AABBox ComputeBoundingBox() const;
};
