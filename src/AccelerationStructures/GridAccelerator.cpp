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
	for (auto currPrimitive : primitivesNotInCells) {
		if (currPrimitive->UsedForInstancing()) {
			continue;
		}
		if (currPrimitive->Intersect(ray, tMin, tMax,
									 intersectionResult)) {
			return currPrimitive;
		}
	}
	
	int ix, iy, iz;
	float dtx, dty, dtz;
	float txNext, tyNext, tzNext;
	bool txInvalid, tyInvalid, tzInvalid;
	int ixStep, iyStep, izStep,
	ixStop, iyStop, izStop;
	// the following code includes modifications
	// from Shirley and Morley (2003)
	// ported to Raytracing from the Ground Up
	RayParameters rayParams(ix, iy, iz, dtx, dty, dtz,
							txNext, tyNext, tzNext,
							ixStep, iyStep, izStep,
							ixStop, iyStop, izStop,
							txInvalid, tyInvalid, tzInvalid);
	if (!CheckBoundsOfRay(ray, tMin, tMax, rayParams)) {
		return nullptr;
	}
	
	while(true) {
		PrimitiveCollection& currentCell = cells[ix + nx*iy + nx*ny*iz];
		if (!txInvalid && txNext < tyNext && txNext < tzNext) {
			auto hitPrimitive =
			   EvaluatePrimitiveCollectionCell(currentCell,ray, tMin, tMax,
											   intersectionResult, txNext);
			if (hitPrimitive != nullptr) {
				return hitPrimitive;
			}
			txNext += dtx;
			ix += ixStep;
			if (ix == ixStop) {
				return nullptr;
			}
		}
		else {
			if (!tyInvalid && tyNext < tzNext) {
				auto hitPrimitive =
				EvaluatePrimitiveCollectionCell(currentCell,ray, tMin, tMax,
												intersectionResult, tyNext);
				if (hitPrimitive != nullptr) {
					return hitPrimitive;
				}
				tyNext += dty;
				iy += iyStep;
				if (iy == iyStop) {
					return nullptr;
				}
			}
			else {
				auto hitPrimitive =
				EvaluatePrimitiveCollectionCell(currentCell,ray, tMin, tMax,
												intersectionResult, tzNext);
				if (hitPrimitive != nullptr) {
					return hitPrimitive;
				}
				tzNext += dtz;
				iz += izStep;
				if (iz == izStop) {
					return nullptr;
				}
			}
		}
	}
	
	return nullptr;
}

bool GridAccelerator::CheckBoundsOfRay(Ray const &ray, float tMin, float tMax,
									   RayParameters& rayParams) {
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
	
	// find closest exit
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
		return false;
	}
	
	// check against bounds of ray
	// entry cannot be greater than max of ray
	if (t0 > tMax) {
		return false;
	}
	// exit cannot be greater than max of ray
	if (t1 > tMax) {
		return false;
	}
	
	// find cell coordinates of initial point
	if (boundingBox.PointInside(rayOrigin)) {
		rayParams.ix = CommonMath::Clamp((originX - x0)*nx/(x1 - x0), 0, nx - 1);
		rayParams.iy = CommonMath::Clamp((originY - y0)*ny/(y1 - y0), 0, ny - 1);
		rayParams.iz = CommonMath::Clamp((originZ - z0)*nz/(z1 - z0), 0, nz - 1);
	}
	else {
		// hit from the outside
		Point3 pointHit = ray.GetPositionAtParam(t0);
		rayParams.ix = CommonMath::Clamp((pointHit[0] - x0)*nx/(x1 - x0), 0, nx - 1);
		rayParams.iy = CommonMath::Clamp((pointHit[1] - y0)*ny/(y1 - y0), 0, ny - 1);
		rayParams.iz = CommonMath::Clamp((pointHit[2] - z0)*nz/(z1 - z0), 0, nz - 1);
	}
	
	// steps along x, y and z (where cell size is a step)
	// t-size/num-cells = size of ray per cell along that dimension
	// size being in parameter space. while the intersections with ray
	// cells are not equally spaced along ray, they are relative
	// per dimension
	rayParams.dtx = (txMax - txMin) / nx;
	rayParams.dty = (tyMax - tyMin) / ny;
	rayParams.dtz = (tzMax - tzMin) / nz;
	
	rayParams.txInvalid = false;
	rayParams.tyInvalid = false;
	rayParams.tzInvalid = false;
	
	// compute next step along ray, parameterized
	// if the direction is positive in world space
	// progress along cells in positive direction
	if (dirX > 0) {
		rayParams.txNext = txMin + (rayParams.ix + 1) * rayParams.dtx;
		rayParams.ixStep = +1;
		rayParams.ixStop = nx;
	}
	else {
		// if the ray starts inside, txMin is behind the ray, so we need to get next
		// point right after the starting cell. So start from
		// txMin, traversing number of cells in-between that and
		// start plus one (nx - ix), and multiply by step length
		rayParams.txNext = txMin + (nx - rayParams.ix) * rayParams.dtx;
		rayParams.ixStep = -1;
		rayParams.ixStop = -1;
	}
	
	// this is assuming the ray is practically vertical in the x-dir
	if (fabs(dirX) < EPSILON) {
		rayParams.txInvalid = true;
		rayParams.ixStep = -1;
		rayParams.ixStop = -1;
	}
	
	if (dirY > 0) {
		rayParams.tyNext = tyMin + (rayParams.iy + 1) * rayParams.dty;
		rayParams.iyStep = +1;
		rayParams.iyStop = ny;
	}
	else {
		rayParams.tyNext = tyMin + (ny - rayParams.iy) * rayParams.dty;
		rayParams.iyStep = -1;
		rayParams.iyStop = -1;
	}
	
	if (fabs(dirY) < EPSILON) {
		rayParams.tyInvalid = true;
		rayParams.iyStep = -1;
		rayParams.iyStop = -1;
	}
	
	if (dirZ > 0) {
		rayParams.tzNext = tzMin + (rayParams.iz + 1) * rayParams.dtz;
		rayParams.izStep = +1;
		rayParams.izStop = nx;
	}
	else {
		rayParams.tzNext = tzMin + (nz - rayParams.iz) * rayParams.dtz;
		rayParams.izStep = -1;
		rayParams.izStop = -1;
	}
	
	if (fabs(dirZ) < EPSILON) {
		rayParams.tzInvalid = true;
		rayParams.izStep = -1;
		rayParams.izStop = -1;
	}
	
	return true;
}

Primitive* GridAccelerator::IntersectAgainstPrimitiveCollection(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax,
	IntersectionResult &intersectionResult) {
	auto & primitives = primitiveCollection.primitives;
	unsigned int numElements = primitives.size();
	Primitive * closestPrimSoFar = nullptr;
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		
		if (currPrimitive->UsedForInstancing()) {
			continue;
		}

		if (currPrimitive->Intersect(ray, tMin, tMax, intersectionResult)) {
			closestPrimSoFar = currPrimitive;
		}
	}
	
	return closestPrimSoFar;
}

Primitive* GridAccelerator::IntersectAgainstPrimitiveCollectionShadow(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax,
	const Primitive* primitiveToExclude) {
	auto & primitives = primitiveCollection.primitives;
	unsigned int numElements = primitives.size();
	Primitive * closestPrimSoFar = nullptr;
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitives[index];
		
		if (currPrimitive == primitiveToExclude ||
			currPrimitive->UsedForInstancing()) {
			continue;
		}

		if (currPrimitive->IntersectShadow(ray, tMin, tMax)) {
			closestPrimSoFar = currPrimitive;
		}
	}
	
	return closestPrimSoFar;
}

bool GridAccelerator::ShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
													 float tMax,
													 const Primitive* primitiveToExclude) {
	for (auto currPrimitive : primitivesNotInCells) {
		if (currPrimitive == primitiveToExclude ||
			currPrimitive->UsedForInstancing()) {
			continue;
		}
		
		if (currPrimitive->IntersectShadow(ray, tMin, tMax)) {
			return currPrimitive;
		}
	}
	
	int ix, iy, iz;
	float dtx, dty, dtz;
	float txNext, tyNext, tzNext;
	bool txHuge, tyHuge, tzHuge;
	int ixStep, iyStep, izStep,
	ixStop, iyStop, izStop;
	// the following code includes modifications
	// from Shirley and Morley (2003)
	// ported to Raytracing from the Ground Up
	RayParameters rayParams(ix, iy, iz, dtx, dty, dtz,
							txNext, tyNext, tzNext,
							ixStep, iyStep, izStep,
							ixStop, iyStop, izStop,
							txHuge, tyHuge, tzHuge);
	if (!CheckBoundsOfRay(ray, tMin, tMax, rayParams)) {
		return nullptr;
	}
	
	while(true) {
		PrimitiveCollection& currentCell = cells[ix + nx*iy + nx*ny*iz];
		if (!txHuge && txNext < tyNext && txNext < tzNext) {
			auto hitPrimitive =
			EvaluatePrimitiveCollectionCellShadow(currentCell,ray, tMin, tMax,
												  txNext, primitiveToExclude);
			if (hitPrimitive != nullptr) {
				return hitPrimitive;
			}
			txNext += dtx;
			ix += ixStep;
			if (ix == ixStop) {
				return nullptr;
			}
		}
		else {
			if (!tyHuge && tyNext < tzNext) {
				auto hitPrimitive =
				EvaluatePrimitiveCollectionCellShadow(currentCell,ray, tMin, tMax,
													  tyNext, primitiveToExclude);
				if (hitPrimitive != nullptr) {
					return hitPrimitive;
				}
				tyNext += dty;
				iy += iyStep;
				if (iy == iyStop) {
					return nullptr;
				}
			}
			else {
				auto hitPrimitive =
				EvaluatePrimitiveCollectionCellShadow(currentCell,ray, tMin, tMax,
													  tzNext, primitiveToExclude);
				if (hitPrimitive != nullptr) {
					return hitPrimitive;
				}
				tzNext += dtz;
				iz += izStep;
				if (iz == izStop) {
					return nullptr;
				}
			}
		}
	}
	
	return nullptr;
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
	
	// compute bounding boxes
	for(auto primitiveCollection : cells) {
		primitiveCollection.ComputeBoundingBox();
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
