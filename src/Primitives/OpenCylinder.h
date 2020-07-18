#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"
#include <iostream>

class OpenCylinder : public Primitive {
public:
	OpenCylinder(float y0, float y1, float radius,
		 std::shared_ptr<Material> const& iMaterial,
		 const std::string& iName) : Primitive(iMaterial, iName),
		y0(y0), y1(y1), radius(radius), invRadius(1.0f/radius) {
	}

	OpenCylinder(float y0, float y1, float radius,
		 std::shared_ptr<Material> && iMaterial,
		const std::string& iName) : Primitive(iMaterial, iName),
		y0(y0), y1(y1), radius(radius), invRadius(1.0f/radius) {
	}
	
	void GenerateBoundingBox();

	bool Intersect(const Ray &ray, float tMin, float& tMax,
				   IntersectionResult &intersectionResult) override;
	bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalAtPosition(IntersectionResult const &intersectionResult) const override {
		return Vector3();
	}
	
	virtual void SamplePrimitive(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override;

private:
	// bottom y value
	float y0;
	// top y value
	float y1;
	float radius;
	float invRadius;
	AABBox boundingBox;
};
