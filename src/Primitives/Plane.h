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
	
	virtual void SamplePrimitiveLocal(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;
	
	virtual bool HasBoundingBox() const override {
		return false;
	}
	
	virtual AABBox GetBoundingBoxLocal() const override {
		return AABBox(); // not valid for planes
	}
	
	virtual AABBox GetBoundingBoxWorld() const override {
		return AABBox(); // not valid for planes
	}
	
protected:
	bool IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
						IntersectionResult &intersectionResult) override;
	bool IntersectShadowLocal(const Ray &rayLocal, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalLocal(IntersectionResult const &intersectionResult)
		const override {
		return normal;
	}

private:
	Point3 origin;
	Vector3 normal;
	float distance;
};
