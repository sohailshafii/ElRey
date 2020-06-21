#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"

class Torus : public Primitive {
public:
	Torus(std::shared_ptr<Material> const & iMaterial,
		  std::shared_ptr<GenericSampler> const & iSampler,
		  const std::string& iName) : Primitive(iMaterial, iSampler, iName),
			sweptRadius(2.0f), tubeRadius(0.5f)
	{
		Initialize();
	}
	
	// Side vectors should NOT be normalized
	Torus(float sweptRadius, float tubeRadius,
		  std::shared_ptr<Material> const & iMaterial,
		  std::shared_ptr<GenericSampler> const & iSampler,
		  const std::string& iName) :
		Primitive(iMaterial, iSampler, iName), sweptRadius(sweptRadius),
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

	bool Intersect(const Ray &ray, float tMin, float& tMax,
					IntersectionResult &intersectionResult) override;
	bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalAtPosition(IntersectionResult const &intersectionResult) const override {
		Vector3 normal;
		float paramSquared = sweptRadiusSquared + tubeRadiusSquared;
	
		auto intersecPos = intersectionResult.GetIntersectionPos();
		float x = intersecPos[0];
		float y = intersecPos[1];
		float z = intersecPos[2];
		float sumSquared = x * x + y * y + z * z;
		
		normal[0] = 4.0f * x * (sumSquared - paramSquared);
		normal[1] = 4.0f * y * (sumSquared - paramSquared + 2.0 * sweptRadiusSquared);
		normal[2] = 4.0f * z * (sumSquared - paramSquared);
		normal.Normalize();
		
		return normal;
	}
	
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
	
	void Initialize() {
		sweptRadiusSquared = sweptRadius*sweptRadius;
		tubeRadiusSquared = tubeRadius*tubeRadius;
		boundingBox = AABBox(-sweptRadius - tubeRadius,
							sweptRadius + tubeRadius,
							-tubeRadius, tubeRadius,
							-sweptRadius - tubeRadius,
							sweptRadius + tubeRadius);
	}
};
