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
	}

	OpenCylinder(float y0, float y1, float radius,
				 std::shared_ptr<Material> && iMaterial,
				 const std::string& iName) : Primitive(iMaterial, iName),
		y0(y0), y1(y1), radius(radius), radiusSqr(radius*radius), invRadius(1.0f/radius) {
	}
	
	void GenerateBoundingBox();

	bool Intersect(const Ray &ray, float tMin, float& tMax,
				   IntersectionResult &intersectionResult) override;
	bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormalAtPosition(IntersectionResult const &intersectionResult) const override {
		Ray const & incomingRay = intersectionResult.GetIncomingRay();
		Point3 const & rayOrigin = incomingRay.GetOrigin();
		Vector3 const & rayDir = incomingRay.GetDirection();
		float tIntersec = intersectionResult.GetRayIntersectT();
		
		Vector3 normalVec = Vector3((rayOrigin[0] + tIntersec*rayDir[0])*invRadius, 0.0f,
									(rayOrigin[2] + tIntersec*rayDir[2])*invRadius);
		// inside surface?
		if (-rayDir*normalVec < 0.0f) {
			normalVec = -normalVec;
		}
		normalVec.Normalize();
		return normalVec;
	}
	
	virtual void SamplePrimitive(Point3& resultingSample) override;
	
	virtual float PDF(const IntersectionResult& intersectionResult) const override;
	
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
