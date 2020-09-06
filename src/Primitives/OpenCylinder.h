#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"
#include <iostream>
#include <vector>

class OpenCylinder : public Primitive {
public:
	OpenCylinder(float y0, float y1, float radius,
				 std::shared_ptr<Material> const& iMaterial,
				 const std::string& iName) : Primitive(iMaterial, iName),
		y0(y0), y1(y1), radius(radius), radiusSqr(radius*radius), invRadius(1.0f/radius) {
		GenerateBoundingBox();
	}

	OpenCylinder(float y0, float y1, float radius,
				 std::shared_ptr<Material> && iMaterial,
				 const std::string& iName) : Primitive(iMaterial, iName),
		y0(y0), y1(y1), radius(radius), radiusSqr(radius*radius), invRadius(1.0f/radius) {
		GenerateBoundingBox();
	}
	
	void GenerateBoundingBox();
	
	virtual void SamplePrimitiveLocal(Point3& resultingSample) override;
	
	virtual void SamplePrimitiveWorld(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBoxLocal() const override;

	virtual AABBox GetBoundingBoxWorld() const override;
	
protected:
	bool IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
				   IntersectionResult &intersectionResult) override;
	bool IntersectShadowLocal(const Ray &rayLocal, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalLocal(IntersectionResult const &intersectionResult) const override;
	
private:
	// bottom y value
	float y0;
	// top y value
	float y1;
	float radius, radiusSqr;
	float invRadius;
	AABBox boundingBoxLocal;
	
	bool TestIfTMaxPasses(float originY, float dirY,
						float tVal, float tMin, float tMax) {
		if (tVal > tMin && tVal < tMax) {
			float yIntersec = originY + tVal * dirY;
			return (yIntersec > y0 && yIntersec < y1);
		}
		return false;
	}
	
	bool TestRayAndSetTMax(const Ray &rayLocal, float tMin, float& tMax);
};
