#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"

class Rectangle : public Primitive {
public:
	// Side vectors should NOT be normalized
	Rectangle(const Point3& iOrigin, const Vector3& iSide1Vec,
		const Vector3& iSide2Vec, std::shared_ptr<Material> material,
			  std::shared_ptr<GenericSampler> sampler) :
		Primitive(material, sampler), origin(iOrigin) {
			side1Vec = iSide1Vec;
			side2Vec = iSide2Vec;
			side1LengthSqr = side1Vec.NormSqr();
			side2LengthSqr = side2Vec.NormSqr();
			area = sqrt(side1LengthSqr) * sqrt(side2LengthSqr);
			inverseArea = 1.0f / area;
			normal = side1Vec ^ side2Vec;
			normal.Normalize();

			sampler->MapSamplesToUnitDisk();
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
	float area, inverseArea;
	Vector3 normal;
	float distance;
};
