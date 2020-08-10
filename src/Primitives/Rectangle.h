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

	Rectangle(const Point3& iOrigin, const Vector3& iSide1Vec,
		const Vector3& iSide2Vec, std::shared_ptr<Material> && iMaterial,
		std::shared_ptr<GenericSampler> && iSampler,
		const std::string& iName) :
		Primitive(iMaterial, iSampler, iName), origin(iOrigin) {
		Initialize(iSide1Vec, iSide2Vec);
	}

	bool IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
					IntersectionResult &intersectionResult) override;
	bool IntersectShadowLocal(const Ray &rayLocal, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalWorld(IntersectionResult const &intersectionResult) const override {
		return isTransformed ? GetWorldToLocalTransposeDir(normal) : normal;
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
	void Initialize(const Vector3& iSide1Vec, const Vector3& iSide2Vec);
	
	AABBox ComputeBoundingBoxLocal() const;

	Point3 origin;
	Vector3 side1Vec, side2Vec;
	float side1LengthSqr, side2LengthSqr;
	float area, inverseArea;
	Vector3 normal;
	AABBox boundingBoxLocal;
};
