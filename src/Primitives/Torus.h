#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"

class Torus : public Primitive {
public:
	Torus(std::shared_ptr<Material> const & iMaterial,
		  const std::string& iName) : Primitive(iMaterial, iName),
			sweptRadius(2.0f), tubeRadius(0.5f)
	{
		Initialize();
	}
	
	// Side vectors should NOT be normalized
	Torus(float sweptRadius, float tubeRadius,
		  std::shared_ptr<Material> const & iMaterial,
		  const std::string& iName) :
		Primitive(iMaterial, iName), sweptRadius(sweptRadius),
		tubeRadius(tubeRadius) {
		Initialize();
	}
	
	virtual bool Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(IntersectionResult const &intersectionResult) const override;
	
	virtual Vector3 GetNormalAtPosition(Point3 const &position) const override;

	virtual void SamplePrimitive(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override;
	
private:
	AABBox boundingBox;
	float sweptRadius;
	float tubeRadius;
	
	float sweptRadiusSquared;
	float tubeRadiusSquared;
	
	void Initialize();
};
