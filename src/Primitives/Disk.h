#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"
#include <iostream>

class Disk : public Primitive {
public:
	Disk(Point3 const & center,
		 Vector3 const & normal, float radius,
		 std::shared_ptr<Material> const& iMaterial,
		 const std::string& iName) : Primitive(iMaterial, iName),
		center(center), normalVec(normal), radiusSquared(radius*radius) {
		normalVec.Normalize();
	}

	Disk(Point3 const & center,
		 Vector3 const & normal, float radius,
		 std::shared_ptr<Material> && iMaterial,
		const std::string& iName) : Primitive(iMaterial, iName),
		center(center), normalVec(normal), radiusSquared(radius*radius) {
		normalVec.Normalize();
	}
	
	void GenerateBoundingBox();

	bool Intersect(const Ray &ray, float tMin, float& tMax,
				   IntersectionResult &intersectionResult) override;
	bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalAtPosition(const Point3& position) const override {
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
	Vector3 normalVec;
	float radiusSquared;
	AABBox boundingBox;
};
