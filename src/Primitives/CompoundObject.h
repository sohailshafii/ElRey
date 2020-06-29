#pragma once

#include "Primitive.h"

class CompoundObject : public Primitive {
public:
	CompoundObject(std::shared_ptr<Material> & iMaterial,
			const std::string& iName) :
		Primitive(iMaterial, iName) {
	}

	CompoundObject(std::shared_ptr<Material> && iMaterial,
		const std::string& iName) :
		Primitive(iMaterial, iName) {
	}

	bool Intersect(const Ray &ray, float tMin, float& tMax,
					IntersectionResult &intersectionResult) override;
	bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalAtPosition(IntersectionResult const &intersectionResult) const override {
		return Vector3(0.0f, 0.0f, 0.0f);
	}
	
	virtual void SamplePrimitive(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override {
		return boundingBox;
	}

private:
	AABBox boundingBox;
};

