#pragma once

#include "Math/Point3.h"
#include "Math/AABBox.h"
#include "Primitives/Primitive.h"
#include "AccelerationStructures/BaseAccelerator.h"
#include <vector>

#include "Primitives/CompoundObject.h"
#include "Primitives/InstancePrimitive.h"

#include "Primitives/Sphere.h"

class GridAccelerator : public BaseAccelerator {
public:
	GridAccelerator();
	GridAccelerator(Primitive **primitives,
					unsigned int numPrimitives);
	
	virtual void SetUpAccelerator(nlohmann::json const & jsonObj) override;
	
	virtual Primitive* Intersect(const Ray &ray, float tMin, float& tMax,
								IntersectionResult &intersectionResult) override;
	
	virtual Primitive* ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
													  float tMax) override;
	
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
		int ix, iy, iz;
		float dtx, dty, dtz;
		float txNext, tyNext, tzNext;
		int ixStep, iyStep, izStep;
		int ixStop, iyStop, izStop;
		bool txInvalid, tyInvalid, tzInvalid;
	};
	
	void SetupCells(nlohmann::json const & jsonObj);
	Point3 GetMinCoordinates();
	Point3 GetMaxCoordinates();
	
	bool CheckBoundsOfRay(Ray const& ray, float tMin, float tMax,
		RayParameters& rayParams);
	
	Primitive* EvaluatePrimitiveCollectionCell(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax, IntersectionResult &intersectionResult, float tNext);
	
	Primitive* IntersectAgainstPrimitiveCollection(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax, IntersectionResult &intersectionResult);
	
	Primitive* IntersectAgainstPrimitiveCollectionShadow(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float tMax);
	
	Primitive* BruteForceIntersect(const Ray &ray, float tMin, float& tMax,
							 IntersectionResult &intersectionResult);
	Primitive* BruteForceShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
														float tMax,
														const Primitive* primitiveToExclude);
	
	AABBox boundingBox;
	// primitives that are not in cells, because they don't have
	// bounding boxes, like planes
	std::vector<Primitive*> primitivesNotInCells;
	std::vector<PrimitiveCollection> cells;
	int nx, ny, nz;
};
