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
	
private:
	Point3 center;
	float radius, radiusSqr;
	AABBox boundingBoxLocal;
	
	AABBox ComputeBoundingBoxLocal() const;
};
