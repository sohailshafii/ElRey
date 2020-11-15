#pragma once

#include "Math/Point3.h"
#include "Math/AABBox.h"
#include "Primitives/Primitive.h"
#include <vector>

class GridAccelerator {
public:
	
	GridAccelerator();
	
private:
	void SetupCells();
	Point3 GetMinCoordinates();
	Point3 GetMaxCoordinates();
	
	AABBox boundingBox;
	std::vector<Primitive*> cells;
	std::vector<Primitive*> primitives;
	int nx, ny, nz;
};
