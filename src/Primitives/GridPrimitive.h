#pragma once

#include "Primitive.h"
#include "Math/Point3.h"
#include "Math/Vector3.h"
#include <vector>

// TODO: build functions
class GridPrimitive : public Primitive {
public:
	// Side vectors should NOT be normalized
	GridPrimitive(const std::string& iName) :
		Primitive(iName) {
	}
	
	virtual bool Intersect(const Ray &ray, float tMin, float& tMax,
						   IntersectionResult &intersectionResult) override;
	virtual bool IntersectShadow(const Ray &ray, float tMin, float tMax) override;
	
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
	
	Primitive* EvaluatePrimitiveCollectionCell(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax, IntersectionResult &intersectionResult, float tNext) {
		// don't set intersection results and tMax until we are tested against tNext
		float tMaxTest = tMax;
		IntersectionResult intersecResTemp;
		auto hitPrimitive = IntersectAgainstPrimitiveCollection(primitiveCollection,
																ray, tMin, tMaxTest,
																intersecResTemp);
		if (hitPrimitive != nullptr && tMaxTest < tNext) {
			tMax = tMaxTest;
			// TODO: try to avoid copy somehow, this is gross (but necessary)
			intersectionResult = intersecResTemp;
			return hitPrimitive;
		}
		
		return nullptr;
	}
	
	bool EvaluatePrimitiveCollectionCellShadow(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float tMax, const Primitive* primitiveToExclude) {
		return IntersectAgainstPrimitiveCollectionShadow(primitiveCollection,
														 ray, tMin, tMax,
														 primitiveToExclude);
	}
	
	Primitive* IntersectAgainstPrimitiveCollection(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax, IntersectionResult &intersectionResult);
	
	bool IntersectAgainstPrimitiveCollectionShadow(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float tMax,
		const Primitive* primitiveToExclude);
	
	Primitive* BruteForceIntersect(const Ray &ray, float tMin, float& tMax,
								   IntersectionResult &intersectionResult);
	bool BruteForceShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
												  float tMax,
												  const Primitive* primitiveToExclude);
	
	AABBox boundingBox;
	// primitives that are not in cells, because they don't have
	// bounding boxes, like planes
	std::vector<Primitive*> primitivesNotInCells;
	std::vector<PrimitiveCollection> cells;
	int nx, ny, nz;
};
