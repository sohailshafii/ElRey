#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"

class Plane : public Primitive {
public:
	Plane(const Point3& iOrigin, const Vector3& iNormal,
		std::shared_ptr<Material> material) : Primitive(material),
		origin(iOrigin), normal(iNormal) {
		this->distance = -origin*normal;
	}

	bool Intersect(const Ray &ray, float tMin, float& tMax,
				   IntersectionResult &intersectionResult) override;
	bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalAtPosition(const Point3& position) const override {
		return normal;
	}
	
	virtual void SamplePrimitive(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;

private:
	Point3 origin;
	Vector3 normal;
	float distance;
};
