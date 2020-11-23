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
	
	class RayParameters {
	public:
		RayParameters(int ix, int iy, int iz,
					 float dtx, float dty, float dtz,
					 float txNext, float tyNext, float tzNext,
					 int ixStep, int iyStep, int izStep,
					 int ixStop, int iyStop, int izStop,
					 bool txHuge, bool tyHuge, bool tzHuge) {
			this->ix = ix;
			this->iy = iy;
			this->iz = iz;
			this->dtx = dtx;
			this->dty = dty;
			this->dtz = dtz;
			this->txNext = txNext;
			this->tyNext = tyNext;
			this->tzNext = tzNext;
			
			this->ixStep = ixStep;
			this->iyStep = iyStep;
			this->izStep = izStep;
			this->ixStop = ixStop;
			this->iyStop = iyStop;
			this->izStop = izStop;
			
			this->txHuge = txHuge;
			this->tyHuge = tyHuge;
			this->tzHuge = tzHuge;
		}
		
		int ix, iy, iz;
		float dtx, dty, dtz;
		float txNext, tyNext, tzNext;
		int ixStep, iyStep, izStep;
		int ixStop, iyStop, izStop;
		bool txHuge, tyHuge, tzHuge;
	};
	
	void SetupCells();
	Point3 GetMinCoordinates();
	Point3 GetMaxCoordinates();
	
	bool CheckBoundsOfRay(Ray const &ray, float tMin, float tMax,
						  RayParameters& rayParams);
	
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
	
	Primitive* EvaluatePrimitiveCollectionCellShadow(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax, float tNext, const Primitive* primitiveToExclude) {
		float tMaxTest = tMax;
		auto hitPrimitive = IntersectAgainstPrimitiveCollectionShadow(primitiveCollection,
																ray, tMin, tMaxTest, primitiveToExclude);
		if (hitPrimitive != nullptr && tMax < tNext) {
			tMax = tMaxTest;
			return hitPrimitive;
		}
		
		return nullptr;
	}
	
	Primitive* IntersectAgainstPrimitiveCollection(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax, IntersectionResult &intersectionResult);
	Primitive* IntersectAgainstPrimitiveCollectionShadow(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax,
		const Primitive* primitiveToExclude);
	
	AABBox boundingBox;
	// primitives that are not in cells, because they don't have
	// bounding boxes, like planes
	std::vector<Primitive*> primitivesNotInCells;
	std::vector<PrimitiveCollection> cells;
	int nx, ny, nz;
};
