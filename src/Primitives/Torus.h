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

	Torus(float sweptRadius, float tubeRadius,
			  const Vector3& iSide2Vec, std::shared_ptr<Material> && iMaterial,
			  std::shared_ptr<GenericSampler> && iSampler,
			  const std::string& iName) :
		Primitive(iMaterial, iSampler, iName), sweptRadius(sweptRadius),
		tubeRadius(tubeRadius) {
		Initialize();
	}

	virtual void SamplePrimitiveLocal(Point3& resultingSample) override;
	
	virtual void SamplePrimitiveWorld(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBoxLocal() const override;
	
	virtual AABBox GetBoundingBoxWorld() const override;

protected:
	virtual bool IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
								IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadowLocal(const Ray &rayLocal, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalLocal(IntersectionResult const &intersectionResult) const override;
	
private:
	AABBox boundingBox;
	float sweptRadius;
	float tubeRadius;
	
	float sweptRadiusSquared;
	float tubeRadiusSquared;
	
	void Initialize();
};
