#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"
#include <vector>

class GridPrimitive : public Primitive {
public:
	GridPrimitive(const std::string& iName) :
		Primitive(iName) {
	}
	
	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
								 IntersectionResult &intersectionResult) override;
	virtual Primitive* IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
	virtual Vector3 GetNormal(const ShadingInfo& shadingInfo) const override;
	
	virtual Vector3 ComputeHardNormal(Point3 const &position) const override;
	
	virtual void SamplePrimitive(Point3& resultingSample,
								 const ShadingInfo& shadingInfo) override;
	
	virtual float PDF(const ShadingInfo& shadingInfo) const override;
	
	virtual bool HasBoundingBox() const override {
		return true;
	}
	
	virtual AABBox GetBoundingBox() const override;
	
private:
	class PrimitiveCollection {
	public:
		void ComputeBoundingBox() {
			boundingBox.Reset();
			for(auto primitive : primitives) {
				boundingBox.Superset(primitive->GetBoundingBox());
			}
		}
		
		AABBox const & GetBoundingBox() {
			return boundingBox;
		}
		
		AABBox boundingBox;
		std::vector<Primitive*> primitives;
	};
	
	class RayParameters {
	public:
		int ix, iy, iz;
		float dtx, dty, dtz;
		float txNext, tyNext, tzNext;
		int ixStep, iyStep, izStep;
		int ixStop, iyStop, izStop;
		bool txInvalid, tyInvalid, tzInvalid;
	};
	
	Point3 ComputeMinCoordinates(std::vector<Primitive*> const & primitives);
	Point3 ComputeMaxCoordinates(std::vector<Primitive*> const & primitives);
	
	bool CheckBoundsOfRay(Ray const& ray, float tMin, float tMax,
		RayParameters& rayParams);
	
	Primitive* EvaluatePrimitiveCollectionCell(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax, IntersectionResult &intersectionResult, float tNext);
	
	Primitive* IntersectAgainstPrimitiveCollection(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax, IntersectionResult &intersectionResult);
	
	Primitive* IntersectAgainstPrimitiveCollectionShadow(PrimitiveCollection & primitiveCollection,
														 const Ray &ray, float tMin, float tMax);
	
	Primitive* BruteForceIntersect(const Ray &ray, float tMin, float& tMax,
								   IntersectionResult &intersectionResult);
	Primitive* BruteForceShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
														float tMax);
	
	AABBox boundingBox;
	// primitives that are not in cells, because they don't have
	// bounding boxes, like planes
	std::vector<Primitive*> primitivesNotInCells;
	std::vector<PrimitiveCollection> cells;
	int nx, ny, nz;
};
