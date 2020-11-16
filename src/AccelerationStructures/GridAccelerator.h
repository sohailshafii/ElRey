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
		AABBox GetBoundingBox() {
			AABBox boundingBox;
			for(auto primitive : primitives) {
				boundingBox.Superset(primitive->GetBoundingBox());
			}
		}
		
		std::vector<Primitive*> primitives;
	};
	
	void SetupCells();
	Point3 GetMinCoordinates();
	Point3 GetMaxCoordinates();
	
	AABBox boundingBox;
	std::vector<PrimitiveCollection> cells;
	int nx, ny, nz;
};
