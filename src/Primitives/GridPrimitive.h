#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"

class GridPrimitive : public Primitive {
public:
	// Side vectors should NOT be normalized
	GridPrimitive(const std::string& iName) :
		Primitive(iName) {
	}
	
	virtual bool Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(ParamsForNormal const &paramsForNormal) const override;
	
	virtual Vector3 ComputeHardNormal(Point3 const &position) const override;
	
	virtual void SamplePrimitive(Point3& resultingSample,
								 IntersectionResult const & intersectionResult) override;
	
	virtual float PDF(ParamsForNormal const &paramsForNormal) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override;
	
private:
	void ComputeBoundingBox();

	AABBox boundingBox;
};
