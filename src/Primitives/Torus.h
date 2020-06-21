#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"

class Torus : public Primitive {
public:
	// Side vectors should NOT be normalized
	Torus(float sweptRadius, float tubeRadius,
		  std::shared_ptr<Material> const & iMaterial,
		  std::shared_ptr<GenericSampler> const & iSampler,
		  const std::string& iName) :
		Primitive(iMaterial, iSampler, iName), sweptRadius(sweptRadius),
		tubeRadius(tubeRadius) {
			sweptRadiusSquared = sweptRadius*sweptRadius;
			tubeRadiusSquared = tubeRadius*tubeRadius;
			boundingBox = AABBox(-sweptRadius - tubeRadius,
								sweptRadius + tubeRadius,
								-tubeRadius, tubeRadius,
								-sweptRadius - tubeRadius,
								sweptRadius + tubeRadius);
	}

	Torus(float sweptRadius, float tubeRadius,
			  const Vector3& iSide2Vec, std::shared_ptr<Material> && iMaterial,
			  std::shared_ptr<GenericSampler> && iSampler,
			  const std::string& iName) :
		Primitive(iMaterial, iSampler, iName), sweptRadius(sweptRadius),
		tubeRadius(tubeRadius) {
			sweptRadiusSquared = sweptRadius*sweptRadius;
			tubeRadiusSquared = tubeRadius*tubeRadius;
			boundingBox = AABBox(-sweptRadius - tubeRadius,
								sweptRadius + tubeRadius,
								-tubeRadius, tubeRadius,
								-sweptRadius - tubeRadius,
								sweptRadius + tubeRadius);
	}

	bool Intersect(const Ray &ray, float tMin, float& tMax,
					IntersectionResult &intersectionResult) override;
	bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalAtPosition(IntersectionResult const &intersectionResult) const override {
		auto intersectionPos = intersectionResult.GetIntersectionPos();
		Vector3 normalVec(intersectionPos[0], intersectionPos[1],
						  intersectionPos[2]);
		return normalVec; // TODO: fix
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
};
