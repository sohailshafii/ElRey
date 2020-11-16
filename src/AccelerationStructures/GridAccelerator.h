#pragma once

#include "Math/Point3.h"
#include "Math/AABBox.h"
#include "Primitives/Primitive.h"
#include <vector>

class GridAccelerator {
public:
	GridAccelerator();
	
	void SetUp(std::vector<Primitive*> const & primitives);
	
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
	
	void SetupCells(std::vector<Primitive*> const & primitives);
	Point3 GetMinCoordinates(std::vector<Primitive*> const & primitives);
	Point3 GetMaxCoordinates(std::vector<Primitive*> const & primitives);
	
	AABBox boundingBox;
	std::vector<PrimitiveCollection> cells;
	int nx, ny, nz;
};
