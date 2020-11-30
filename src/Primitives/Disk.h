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
		GenerateBoundingBox();
	}
	
	virtual bool Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(ParamsForNormal const &paramsForNormal) const override;
	
	//virtual Vector3 GetNormalAtPosition(Point3 const &position) const override;
	
	void GenerateBoundingBox();

	virtual void SamplePrimitive(Point3& resultingSample,
								 IntersectionResult const & intersectionResult) override;
	
	virtual float PDF(ParamsForNormal const &paramsForNormal) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override;
	
private:
	Point3 center;
	Vector3 normalVec;
	float radiusSquared;
	AABBox boundingBoxLocal;
};
