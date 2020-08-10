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

	Disk(Point3 const & center,
		 Vector3 const & normal, float radius,
		 std::shared_ptr<Material> && iMaterial,
		const std::string& iName) : Primitive(iMaterial, iName),
		center(center), normalVec(normal), radiusSquared(radius*radius) {
		normalVec.Normalize();
		GenerateBoundingBox();
	}
	
	void GenerateBoundingBox();

	bool IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
				   IntersectionResult &intersectionResult) override;
	bool IntersectShadowLocal(const Ray &rayLocal, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalWorld(IntersectionResult const &intersectionResult)
		const override {
		return isTransformed ? GetWorldToLocalTransposeDir(normalVec) : normalVec;
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
	Point3 center;
	Vector3 normalVec;
	float radiusSquared;
	AABBox boundingBoxLocal;
};
