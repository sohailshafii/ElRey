#include "GridAccelerator.h"
#include "CommonMath.h"
#include "Primitive.h"
#include <iostream>

GridAccelerator::GridAccelerator() : BaseAccelerator() {
}

GridAccelerator::GridAccelerator(Primitive **primitives,
								 unsigned int numPrimitives)
	: BaseAccelerator(primitives, numPrimitives) {
}

Primitive* GridAccelerator::Intersect(const Ray &ray, float tMin, float& tMax,
								IntersectionResult &intersectionResult) {
	Primitive* closestPrimitive = nullptr;
	
	for (auto currPrimitive : primitivesNotInCells) {
		if (currPrimitive->Intersect(ray, tMin, tMax,
									 intersectionResult)) {
			return currPrimitive;
		}
	}
	
	Point3 const & rayOrigin = ray.GetOrigin();
	Vector3 const & rayDirection = ray.GetDirection();
	float originX = rayOrigin[0];
	float originY = rayOrigin[1];
	float originZ = rayOrigin[2];
	float dirX = rayDirection[0];
	float dirY = rayDirection[1];
	float dirZ = rayDirection[2];
	
	float x0 = boundingBox.x0;
	float y0 = boundingBox.y0;
	float z0 = boundingBox.z0;
	float x1 = boundingBox.x1;
	float y1 = boundingBox.y1;
	float z1 = boundingBox.z1;
	
	float txMin, tyMin, tzMin;
	float txMax, tyMax, tzMax;
	
	// the following code includes modifications
	// from Shirley and Morley (2003)
	// from Raytracing from the Ground Up
	
	// find intersections with bounds
	// in x, y and z directions
	float a = 1.0f / dirX;
	if (a >= 0) {
		txMin = (x0 - originX) * a;
		txMax = (x1 - originX) * a;
	}
	else {
		txMin = (x1 - originX) * a;
		txMax = (x0 - originX) * a;
	}
	
	float b = 1.0 / dirY;
	if (b >= 0) {
		tyMin = (y0 - originY) * b;
		tyMax = (y1 - originY) * b;
	}
	else {
		tyMin = (y1 - originY) * b;
		tyMax = (y0 - originY) * b;
	}
	
	float c = 1.0 / dirZ;
	if (c >= 0) {
		tzMin = (z0 - originZ) * c;
		tzMax = (z1 - originZ) * c;
	}
	else {
		tzMin = (z1 - originZ) * c;
		tzMax = (z0 - originZ) * c;
	}
	
	float t0, t1;
	
	// find largest entry
	if (txMin > tyMin) {
		t0 = txMin;
	}
	else {
		t0 = tyMin;
	}
	
	if (tzMin > t0) {
		t0 = tzMin;
	}
	
	// find closest entry
	if (txMax < tyMax) {
		t1 = txMax;
	}
	else {
		t1 = tyMax;
	}
	
	if (tzMax < t1) {
		t1 = tzMax;
	}
	
	// if entry is larger than exit, return false
	if (t0 > t1) {
		return nullptr;
	}
	
	// find initial cell coordinates
	int ix, iy, iz;
	if (boundingBox.PointInside(rayOrigin)) {
		ix = CommonMath::Clamp((originX - x0)*nx/(x1 - x0), 0, nx - 1);
		iy = CommonMath::Clamp((originY - y0)*ny/(y1 - y0), 0, ny - 1);
		iz = CommonMath::Clamp((originZ - z0)*nz/(z1 - z0), 0, nz - 1);
	}
	else {
		// hit from the outside
		Point3 pointHit = ray.GetPositionAtParam(t0);
		ix = CommonMath::Clamp((pointHit[0] - x0)*nx/(x1 - x0), 0, nx - 1);
		iy = CommonMath::Clamp((pointHit[1] - y0)*ny/(y1 - y0), 0, ny - 1);
		iz = CommonMath::Clamp((pointHit[2] - z0)*nz/(z1 - z0), 0, nz - 1);
	}
	
	// steps along x, y and z (where cell size is a step)
	// t-size/num-cells = size of ray per cell along that dimension
	float dtx = (txMax - txMin) / nx;
	float dty = (tyMax - tyMin) / ny;
	float dtz = (tzMax - tzMin) / nz;
	
	float txNext, tyNext, tzNext;
	int ixStep, iyStep, izStep;
	int ixStop, iyStop, izStop;
	
	if (dirX > 0) {
		txNext = txMin + (ix + 1) * dtx;
		ixStep = +1;
		ixStop = nx;
	}
	else {
		// if nx is 100, and ix is 3, nx-ix = 97??
		txNext = txMin + (nx - ix) * dtx;
		ixStep = -1;
		ixStop = -1;
	}
	
	if (fabs(dirX) < EPSILON) {
		// TODO boolean
		txNext = 1000000000.0;
		ixStep = -1;
		ixStop = -1;
	}
	
	if (dirY > 0) {
		tyNext = tyMin + (iy + 1) * dty;
		iyStep = +1;
		iyStop = ny;
	}
	else {
		tyNext = tyMin + (ny - iy) * dty;
		iyStep = -1;
		iyStop = -1;
	}
	
	if (fabs(dirY) < EPSILON) {
		// TODO boolean
		tyNext = 1000000000.0;
		iyStep = -1;
		iyStop = -1;
	}
	
	if (dirZ > 0) {
		tzNext = tzMin + (iz + 1) * dtz;
		izStep = +1;
		izStop = nx;
	}
	else {
		tzNext = tzMin + (nz - iz) * dtz;
		izStep = -1;
		izStop = -1;
	}
	
	if (fabs(dirZ) < EPSILON) {
		// TODO boolean
		tzNext = 1000000000.0;
		izStep = -1;
		izStop = -1;
	}
	
	// TODO: re-write to use grid
	for (auto currentPrimitive : primitives) {
		if (currentPrimitive->UsedForInstancing()) {
			continue;
		}
		auto hitPrimitive = currentPrimitive->Intersect(ray, tMin, tMax,
														intersectionResult);
		if (hitPrimitive) {
			closestPrimitive = currentPrimitive;
		}
	}
	return closestPrimitive;
}

bool GridAccelerator::ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
													 float tMax,
													 const Primitive* primitiveToExclude) {
	// TODO: re-write to use grid
	Ray rayToCast = ray;
	Vector3 originalDir = ray.GetDirection();
	Point3 originalOrigin = ray.GetOrigin();
	for (auto currentPrimitive : primitives) {
		if (currentPrimitive == primitiveToExclude ||
			currentPrimitive->UsedForInstancing()) {
			continue;
		}
		
		if (currentPrimitive->IntersectShadow(ray, tMin, tMax))
		{
			return true;
		}
	}
	return false;
}

void GridAccelerator::SetUpAccelerator() {
	SetupCells();
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
	float xConversionFactor = nx/(p1[0] - p0[0]);
	float yConversionFactor = ny/(p1[1] - p0[1]);
	float zConversionFactor = nz/(p1[2] - p0[2]);
	int zSliceSize = nx*ny;
	for (size_t primIndex = 0; primIndex < numPrimitives; primIndex++) {
		Primitive* currPrimitive = primitives[primIndex];
		if (!currPrimitive->HasBoundingBox()) {
			primitivesNotInCells.push_back(currPrimitive);
			continue;
		}
		
		objectBBox = currPrimitive->GetBoundingBox();
		// compute the cell indices at the corners of
		// the bounding box of the object
		int ixMin = CommonMath::Clamp((objectBBox.x0 - p0[0])*xConversionFactor, 0, nx - 1);
		int iyMin = CommonMath::Clamp((objectBBox.y0 - p0[1])*yConversionFactor, 0, ny - 1);
		int izMin = CommonMath::Clamp((objectBBox.z0 - p0[2])*zConversionFactor, 0, nz - 1);

		int ixMax = CommonMath::Clamp((objectBBox.x1 - p0[0])*xConversionFactor, 0, nx - 1);
		int iyMax = CommonMath::Clamp((objectBBox.y1 - p0[1])*yConversionFactor, 0, ny - 1);
		int izMax = CommonMath::Clamp((objectBBox.z1 - p0[2])*zConversionFactor, 0, nz - 1);
		
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
	std::cout << "Num zeroes = " << numZeroes << ", num ones = " << numOnes << "  numTwos = " << numTwos << std::endl;
	std::cout << "Num threes = " << numThrees << "  numGreater = " << numGreater << std::endl;
	
	counts.erase(counts.begin(), counts.end());
}

Point3 GridAccelerator::GetMinCoordinates() {
	AABBox objectBBox;
	Point3 minCoord;
	bool xSet = false, ySet = false, zSet = false;
	size_t numPrimitives = primitives.size();
	
	for (size_t index = 0; index < numPrimitives; index++) {
		// skip primitives that don't have a bounding box
		if (!primitives[index]->HasBoundingBox()) {
			continue;
		}
		
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
		// skip primitives that don't have a bounding box
		if (!primitives[index]->HasBoundingBox()) {
			continue;
		}
		
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
