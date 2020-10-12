#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"

class Rectangle : public Primitive {
public:
	// Side vectors should NOT be normalized
	Rectangle(const Point3& iOrigin, const Vector3& iSide1Vec,
		const Vector3& iSide2Vec, std::shared_ptr<Material> const & iMaterial,
			std::shared_ptr<GenericSampler> const & iSampler,
			const std::string& iName) :
		Primitive(iMaterial, iSampler, iName), origin(iOrigin) {
		Initialize(iSide1Vec, iSide2Vec);
	}
	
	virtual bool Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(ParamsForNormal const &paramsForNormal) const override {
		return normal;
	}
	
	virtual Vector3 GetNormalAtPosition(Point3 const &position) const override {
		return normal;
	}
	
	virtual void SamplePrimitive(Point3& resultingSample) override;
	
	virtual float PDF(ParamsForNormal const &paramsForNormal) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override;
	
private:
	void Initialize(const Vector3& iSide1Vec, const Vector3& iSide2Vec);
	
	AABBox ComputeBoundingBox() const;

	Point3 origin;
	Vector3 side1Vec, side2Vec;
	float side1LengthSqr, side2LengthSqr;
	float area, inverseArea;
	Vector3 normal;
	AABBox boundingBoxLocal;
};
