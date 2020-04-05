#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"

class Rectangle : public Primitive {
public:
	Rectangle(const Point3& iOrigin, const Vector3& iSide1Vec,
		const Vector3& iSide2Vec, float iSide1Length,
		float iSide2Length, std::shared_ptr<Material> material,
			  std::shared_ptr<GenericSampler> sampler) :
		Primitive(material, sampler), origin(iOrigin),
		side1LengthSqr(iSide1Length*iSide1Length),
		side2LengthSqr(iSide2Length*iSide2Length) {
			side1Vec = iSide1Vec;
			side2Vec = iSide2Vec;
	}

	bool Intersect(const Ray &ray, float tMin, float& tMax,
				   IntersectionResult &intersectionResult) override;
	bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalAtPosition(const Point3& position) const override {
		return normal;
	}
	
	virtual void SamplePrimitive(Point3& resultingSample) override;
	
	virtual float PDF(IntersectionResult& intersectionResult) const override;

private:
	Point3 origin;
	Vector3 side1Vec, side2Vec;
	float side1LengthSqr, side2LengthSqr;
	Vector3 normal;
	float distance;
};
