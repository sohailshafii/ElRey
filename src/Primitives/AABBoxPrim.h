#pragma once

#include "Primitive.h"

class Point3;
class Point4;
class Ray;

class AABBoxPrim : public Primitive  {
public:
	enum AABoxFace {
		MinX = 0,
		MaxX,
		MinY,
		MaxY,
		MinZ,
		MaxZ
	};
	
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
	
	bool Intersect(const Ray &ray, float tMin, float& tMax,
				IntersectionResult &intersectionResult) override;
	
	bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	bool PointInside(Point3 const& point) const;
	
	bool PointInside(Point4 const& point) const;
	
	virtual Vector3 GetNormalAtPosition(const Point3& position) const override;
	
	virtual void SamplePrimitive(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override {
		return AABBox(x0, y0, z0, x1, y1, z1);
	}
	
private:
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
