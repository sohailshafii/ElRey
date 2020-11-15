#include "GridAccelerator.h"
#include "CommonMath.h"
#include "CompoundObject.h"
#include <iostream>

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
	// s is like volume/object, and we do the
	// cube root of it, to make it per dim
	// multiplying wx by the inverse sorta
	// cancels the volume, so that you are left
	// with number of objects per axis
	nx = multiplier * wx / s + 1;
	ny = multiplier * wy / s + 1;
	nz = multiplier * wz / s + 1;
	
	// grid cells start out with nothing
	int numCells = nx*ny*nz;
	cells.reserve(numPrimitives);
	
	for (size_t index = 0; index < numCells; index++) {
		cells.push_back(PrimitiveCollection());
	}
	
	// temp objects to hold number of objects in each
	// cell
	std::vector<int> counts;
	counts.reserve(numCells);
	for (int cellIndex = 0; cellIndex < numCells; cellIndex++) {
		counts.push_back(0);
	}
	
	AABBox objectBBox;
	int cellArrayIndex;
	float xConversionFactor = nx/(p1[0] - p0[0]);
	float yConversionFactor = ny/(p1[1] - p0[1]);
	float zConversionFactor = nz/(p1[2] - p0[2]);
	int zSliceSize = nx*ny;
	for (size_t primIndex = 0; primIndex < numPrimitives; primIndex++) {
		Primitive* currPrimitive = primitives[primIndex];
		objectBBox = currPrimitive->GetBoundingBox();
		// compute the cell indices at the corners of
		// the bounding box of the object
		int ixMin = CommonMath::Clamp((objectBBox.x0 - p0[0])*xConversionFactor, 0, nx - 1);
		int iyMin = CommonMath::Clamp((objectBBox.y0 - p0[1])*yConversionFactor, 0, ny - 1);
		int izMin = CommonMath::Clamp((objectBBox.z0 - p0[2])*yConversionFactor, 0, nz - 1);

		int ixMax = CommonMath::Clamp((objectBBox.x1 - p0[0])*xConversionFactor, 0, nx - 1);
		int iyMax = CommonMath::Clamp((objectBBox.y1 - p0[1])*yConversionFactor, 0, ny - 1);
		int izMax = CommonMath::Clamp((objectBBox.z1 - p0[2])*yConversionFactor, 0, nz - 1);
		
		// add objects to the cells
		for (int zIndex = izMin; zIndex <= izMax; zIndex++) {
			for (int yIndex = iyMin; yIndex <= iyMax; yIndex++) {
				int yOffset = nx*yIndex;
				for (int xIndex = ixMin; xIndex <= ixMax; xIndex++) {
					int oneDimIndex = xIndex + yOffset + zSliceSize*zIndex;
					cells[oneDimIndex].primitives.push_back(currPrimitive);
					counts[oneDimIndex] += 1;
				}
			}
		}
	}
	
	primitives.erase(primitives.begin(), primitives.end());

	// useful stats
	int numZeroes = 0, numOnes = 0,
		numTwos = 0, numThrees = 0,
		numGreater = 0;
	for (int cellIndex = 0; cellIndex < numCells; cellIndex++) {
		if (counts[cellIndex] == 0) {
			numZeroes++;
		}
		else if (counts[cellIndex] == 1) {
			numOnes++;
		}
		else if (counts[cellIndex] == 1) {
			numTwos++;
		}
		else if (counts[cellIndex] == 2) {
			numThrees++;
		}
		else if (counts[cellIndex] > 3) {
			numGreater++;
		}
	}
	
	std::cout << "Num cells total = " << numCells << std::endl;
	std::cout << "num zeroes = " << numZeroes << ", num ones = " << numOnes << "  numTwos = " << numTwos << std::endl;
	std::cout << "num threes = " << numThrees << "  numGreater = " << numGreater << std::endl;
	
	counts.erase(counts.begin(), counts.end());
}

// TODO make sure all primitives have some sort of bounding
// box for this to work
Point3 GridAccelerator::GetMinCoordinates() {
	AABBox objectBBox;
	Point3 minCoord;
	bool xSet = false, ySet = false, zSet = false;
	size_t numPrimitives = primitives.size();
	
	for (size_t index = 0; index < numPrimitives; index++) {
		objectBBox = primitives[index]->GetBoundingBox();
		if (!xSet || objectBBox.x0 < minCoord[0]) {
			minCoord[0] = objectBBox.x0;
			xSet = true;
		}
		if (!ySet || objectBBox.y0 < minCoord[1]) {
			minCoord[1] = objectBBox.y0;
			ySet = true;
		}
		if (!zSet || objectBBox.z0 < minCoord[2]) {
			minCoord[2] = objectBBox.z0;
			zSet = true;
		}
	}
	
	minCoord[0] -= EPSILON;
	minCoord[1] -= EPSILON;
	minCoord[2] -= EPSILON;
	
	return minCoord;
}

Point3 GridAccelerator::GetMaxCoordinates() {
	AABBox objectBBox;
	Point3 maxCoord;
	bool xSet = false, ySet = false, zSet = false;
	size_t numPrimitives = primitives.size();
	
	for (size_t index = 0; index < numPrimitives; index++) {
		objectBBox = primitives[index]->GetBoundingBox();
		if (!xSet || objectBBox.x1 > maxCoord[0]) {
			maxCoord[0] = objectBBox.x1;
			xSet = true;
		}
		if (!ySet || objectBBox.y1 > maxCoord[1]) {
			maxCoord[1] = objectBBox.y1;
			ySet = true;
		}
		if (!zSet || objectBBox.z1 > maxCoord[2]) {
			maxCoord[2] = objectBBox.z1;
			zSet = true;
		}
	}
	
	maxCoord[0] += EPSILON;
	maxCoord[1] += EPSILON;
	maxCoord[2] += EPSILON;
	
	return maxCoord;
}
