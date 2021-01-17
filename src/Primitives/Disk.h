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
	
	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
								 IntersectionResult &intersectionResult) override;
	virtual Primitive* IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(const ShadingInfo& shadingInfo) const override;
	
	virtual Vector3 ComputeHardNormal(Point3 const &position) const override;
	
	void GenerateBoundingBox();

	virtual void SamplePrimitive(Point3& resultingSample,
								 const ShadingInfo& shadingInfo) override;
	
	virtual float PDF(const ShadingInfo& shadingInfo) const override;
	
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
