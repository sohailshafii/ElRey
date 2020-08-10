#pragma once

#include "Primitive.h"
#include <cstdint>

class Point3;
class Point4;
class Ray;

class AABBoxPrim : public Primitive  {
public:
	AABBoxPrim(std::shared_ptr<Material> const & iMaterial,
			   std::shared_ptr<GenericSampler> const & iSampler,
			   const std::string& iName);
	
	AABBoxPrim(const float x0, const float y0, const float z0,
			   const float x1, const float y1, const float z1,
			   std::shared_ptr<Material> const & iMaterial,
			   std::shared_ptr<GenericSampler> const & iSampler,
			   const std::string& iName);
	
	AABBoxPrim(Point3 const & min, Point3 const & max,
			   std::shared_ptr<Material> const & iMaterial,
			   std::shared_ptr<GenericSampler> const & iSampler,
			   const std::string& iName);
	
	AABBoxPrim(Point4 const & min, Point4 const & max,
			   std::shared_ptr<Material> const & iMaterial,
			   std::shared_ptr<GenericSampler> const & iSampler,
			   const std::string& iName);
	
	bool IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
				IntersectionResult &intersectionResult) override;
	
	bool IntersectShadowLocal(const Ray &rayLocal, float tMin, float tMax) override;
	
	bool PointInsideLocal(Point3 const& point) const;
	
	bool PointInsideLocal(Point4 const& point) const;
	
	virtual Vector3 GetNormalWorld(IntersectionResult const &intersectionResult) const override;
	
	virtual void SamplePrimitiveLocal(Point3& resultingSample) override;
	
	virtual void SamplePrimitiveWorld(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBoxLocal() const override;
	
	virtual AABBox GetBoundingBoxWorld() const override;
	
private:
	enum FaceHit {
		NegativeX = 0,
		NegativeY,
		NegativeZ,
		PositiveX,
		PositiveY,
		PositiveZ
	};
	
	// min point
	float x0, y0, z0;
	// max point
	float x1, y1, z1;
	
	float invVolume;
	Point3 center;
	
	void CalculateInvVolume() {
		invVolume = 1.0f/((x1-x0)*(y1-y0)*(z1-z0));
	}
	
	void CalculateCenter() {
		center = Point3((x0+x1)*0.5f, (y0+y1)*0.5f,
						(z0+z1)*0.5f);
	}
};
