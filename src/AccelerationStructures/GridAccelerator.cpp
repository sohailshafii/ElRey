#include "GridAccelerator.h"

GridAccelerator::GridAccelerator() {
	
}

void GridAccelerator::SetupCells() {
	// find min and max coordinates of the grid
	Point3 p0 = GetMinCoordinates();
	Point3 p1 = GetMaxCoordinates();
	
	// update bounding box with min and max coords
	boundingBox.x0 = p0[0];
	boundingBox.y0 = p0[1];
	boundingBox.z0 = p0[2];
	boundingBox.x1 = p1[0];
	boundingBox.y1 = p1[1];
	boundingBox.z1 = p1[2];
	
	// compute number of cells in the x, y, and z dir
	size_t numPrimitives = primitives.size();
	// compute grid extents in all dimensions
	float wx = p1[0] - p0[0];
	float wy = p1[1] - p0[1];
	float wz = p1[2] - p0[2];
	// use a multiplier that results in about 8x
	// more cells than objects
	float multiplier = 2.0;
	float s = pow(wx * wy * wz / numPrimitives,
				  0.333333);
	nx = multiplier * wx / s + 1;
	ny = multiplier * wy / s + 1;
	nz = multiplier * wz / s + 1;
}

// TODO
Point3 GridAccelerator::GetMinCoordinates() {
	return Point3(0.0f, 0.0f, 0.0f);
}

// TODO
Point3 GridAccelerator::GetMaxCoordinates() {
	return Point3(0.0f, 0.0f, 0.0f);
}
