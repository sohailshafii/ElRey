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
	}

	Sphere(const Point3& iCenter, float iRadius,
		std::shared_ptr<Material> && iMaterial,
		const std::string& iName) : Primitive(iMaterial, iName),
		center(iCenter), radius(iRadius) {
		radiusSqr = radius * radius;
	}

	bool Intersect(const Ray &ray, float tMin, float& tMax,
				   IntersectionResult &intersectionResult) override;
	bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalAtPosition(const Point3& position) const override {
		Vector3 normalVec = (position - center);
		normalVec.Normalize();
		return normalVec;
	}
	
	virtual void SamplePrimitive(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override;

private:
	Point3 center;
	float radius, radiusSqr;
};
