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
	
	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
								 IntersectionResult &intersectionResult) override;
	virtual Primitive* IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(ParamsForNormal const &paramsForNormal) const override;
	
	virtual Vector3 ComputeHardNormal(Point3 const &position) const override;

	virtual void SamplePrimitive(Point3& resultingSample,
								 IntersectionResult const & intersectionResult) override;
	
	virtual float PDF(ParamsForNormal const &paramsForNormal) const override;
	
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
