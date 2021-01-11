#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"

class Plane : public Primitive {
public:
	Plane(const Point3& iOrigin, const Vector3& iNormal,
		std::shared_ptr<Material> const & iMaterial,
		const std::string& iName) : Primitive(iMaterial, iName),
		origin(iOrigin), normal(iNormal) {
		this->distance = -origin*normal;
	}
	
	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
								 IntersectionResult &intersectionResult) override;
	virtual Primitive* IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(ParamsForNormal const &paramsForNormal) const override;
	
	virtual Vector3 ComputeHardNormal(Point3 const &position) const override;
	
	virtual void SamplePrimitive(Point3& resultingSample,
								 IntersectionResult const & intersectionResult) override;
	
	virtual float PDF(ParamsForNormal const &paramsForNormal) const override;
	
	virtual bool HasBoundingBox() const override {
		return false;
	}
	
	virtual AABBox GetBoundingBox() const override {
		return AABBox(); // not valid for planes
	}

private:
	Point3 origin;
	Vector3 normal;
	float distance;
};
