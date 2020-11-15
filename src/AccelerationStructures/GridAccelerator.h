#pragma once

#include "Math/Point3.h"
#include "Math/AABBox.h"
#include "Primitives/Primitive.h"
#include <vector>

class GridAccelerator {
public:
	
	GridAccelerator();
	
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
	std::vector<Primitive*> primitives;
	int nx, ny, nz;
};
