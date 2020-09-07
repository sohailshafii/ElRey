#pragma once

#include "Primitive.h"
#include "Math/Point4.h"
#include <iostream>

class Sphere : public Primitive {
public:
	Sphere(const Point3& iCenter, float iRadius,
			std::shared_ptr<Material> const& iMaterial,
			const std::string& iName) : Primitive(iMaterial, iName),
		center(iCenter), radius(iRadius) {
		radiusSqr = radius*radius;
		boundingBoxLocal = ComputeBoundingBoxLocal();
	}
	
	virtual void SamplePrimitiveLocal(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBoxLocal() const override;
	
	virtual AABBox GetBoundingBoxWorld() const override;

protected:
	bool IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
				   IntersectionResult &intersectionResult) override;
	bool IntersectShadowLocal(const Ray &rayLocal, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalLocal(IntersectionResult const &intersectionResult) const override;
	
private:
	Point3 center;
	float radius, radiusSqr;
	AABBox boundingBoxLocal;
	
	AABBox ComputeBoundingBoxLocal() const;
};
