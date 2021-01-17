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
	
	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
								 IntersectionResult &intersectionResult) override;
	virtual Primitive* IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(const ShadingInfo& shadingInfo) const override;
	
	virtual Vector3 ComputeHardNormal(Point3 const &position) const override;
	
	void GenerateBoundingBox();
	
	virtual void SamplePrimitive(Point3& resultingSample,
								 const ShadingInfo& shadingInfo) override;
	
	virtual float PDF(const ShadingInfo& shadingInfo) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}

	virtual AABBox GetBoundingBox() const override;
	
private:
	// bottom y value
	float y0;
	// top y value
	float y1;
	float radius, radiusSqr;
	float invRadius;
	AABBox boundingBox;
	
	bool TestIfTMaxPasses(float originY, float dirY,
						float tVal, float tMin, float tMax) {
		if (tVal > tMin && tVal < tMax) {
			float yIntersec = originY + tVal * dirY;
			return (yIntersec > y0 && yIntersec < y1);
		}
		return false;
	}
	
	bool TestRayAndSetTMax(const Ray &ray, float tMin, float& tMax);
};
