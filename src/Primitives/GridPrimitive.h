#pragma once

#include "Primitive.h"

class GridPrimitive : public Primitive {
public:
	
	virtual bool Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) override;
	
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(ParamsForNormal const &paramsForNormal) const override;
	
	// useful if you just want to query a normal without intersection information
	virtual Vector3 GetNormalAtPosition(Point3 const &position) const override;
	
	virtual void SamplePrimitive(Point3& resultingSample) override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override {
		return boundingBox;
	}
	
private:
	AABBox boundingBox;
};
