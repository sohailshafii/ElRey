#pragma once

#include "Math/Point3.h"
#include "Math/AABBox.h"
#include "Primitives/Primitive.h"
#include "AccelerationStructures/BaseAccelerator.h"
#include <vector>

class GridAccelerator : public BaseAccelerator {
public:
	GridAccelerator();
	GridAccelerator(Primitive **primitives,
					unsigned int numPrimitives);
	
	virtual void SetUpAccelerator() override;
	
	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
								IntersectionResult &intersectionResult) override;
	
	virtual bool ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
												float tMax,
												const Primitive* primitiveToExclude) override;
	
protected:
	
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
	
	void SetupCells();
	Point3 GetMinCoordinates();
	Point3 GetMaxCoordinates();
	
	Primitive* EvaluatePrimitiveCollectionCell(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax, IntersectionResult &intersectionResult, float tNext) {
		IntersectionResult intersectionResultTest = intersectionResult;
		float tMaxTest = tMax;
		auto hitPrimitive = IntersectAgainstPrimitiveCollection(primitiveCollection,
																ray, tMin, tMaxTest,
																intersectionResultTest);
		if (hitPrimitive != nullptr && tMax < tNext) {
			intersectionResult = intersectionResultTest;
			tMax = tMaxTest;
			return hitPrimitive;
		}
		
		return nullptr;
	}
	
	Primitive* IntersectAgainstPrimitiveCollection(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax,
												   IntersectionResult &intersectionResult);
	
	AABBox boundingBox;
	// primitives that are not in cells, because they don't have
	// bounding boxes, like planes
	std::vector<Primitive*> primitivesNotInCells;
	std::vector<PrimitiveCollection> cells;
	int nx, ny, nz;
};
