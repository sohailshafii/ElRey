
#include "GridPrimitive.h"
#include "Math/CommonMath.h"
#include "SceneData/CommonLoaderFunctions.h"
#include <iostream>

//#define BRUTE_FORCE_TEST

GridPrimitive::~GridPrimitive() {
}

void GridPrimitive::SetUpAccelerator(float multipier,
									 const std::vector<std::shared_ptr<Primitive>> & primitives) {
	std::cout << "Setting up Grid...\n";
	SetupCells(multipier, primitives);
	std::cout << "Done!\n";
}

void GridPrimitive::SetupCells(float multipier,
							   const std::vector<std::shared_ptr<Primitive>> & primitives) {
	// find min and max coordinates of the grid
	Point3 p0 = ComputeMinCoordinates(primitives);
	Point3 p1 = ComputeMaxCoordinates(primitives);
	
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
	// use a multiplier to increase cell count
	
	float multiplier = 1.0f;
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
	unsigned int numPrimitivesAdded;
	for (size_t primIndex = 0; primIndex < numPrimitives; primIndex++) {
		std::shared_ptr<Primitive> currPrimitive = primitives[primIndex];
		
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
					numPrimitivesAdded++;
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
	std::cout << "Num intersections with cells in grid " << numPrimitivesAdded <<
	" vs original pimitives " << numPrimitives << ". Num not in cells: "
		<< primitivesNotInCells.size()  << ".\n";
	
	counts.erase(counts.begin(), counts.end());
}

Primitive* GridPrimitive::Intersect(const Ray &ray, float tMin, float& tMax,
									IntersectionResult &intersectionResult) {
	Primitive* closestPrimitive = nullptr;
	bool hitBefore = false;
	for (auto currPrimitive : primitivesNotInCells) {
		IntersectionResult currResult;
		auto hitPrim = currPrimitive->Intersect(ray, tMin, tMax,
												currResult);
		if (hitPrim != nullptr) {
			closestPrimitive = hitPrim;
			intersectionResult = currResult;
			intersectionResult.compoundPrimitiveToIntersectedPrim[this] =
				closestPrimitive;
			hitBefore = true;
		}
	}
#ifdef BRUTE_FORCE_TEST
	Primitive* closestFromBruteForce = BruteForceIntersect(ray, tMin, tMax, intersectionResult);
	if (closestFromBruteForce != nullptr) {
		return closestFromBruteForce;
	}
	else {
		return closestPrimitive;
	}
#endif
	
	// the following code includes modifications
	// from Shirley and Morley (2003)
	// ported to Raytracing from the Ground Up
	RayParameters rayParams;
	if (!CheckBoundsOfRay(ray, tMin, tMax, rayParams)) {
		return closestPrimitive;
	}
	
	while(true) {
		PrimitiveCollection& currentCell = cells[rayParams.ix +
			nx* rayParams.iy + nx*ny* rayParams.iz];
		if (!rayParams.txInvalid && rayParams.txNext < rayParams.tyNext &&
			rayParams.txNext < rayParams.tzNext) {
			auto hitPrimitive =
			   EvaluatePrimitiveCollectionCell(currentCell, ray, tMin, tMax,
											   intersectionResult, rayParams.txNext);
			if (hitPrimitive != nullptr) {
				closestPrimitive = hitPrimitive;
				break;
			}
			
			rayParams.txNext += rayParams.dtx;
			rayParams.ix += rayParams.ixStep;
			if (rayParams.ix == rayParams.ixStop) {
				break;
			}
		}
		else if (!rayParams.tyInvalid && rayParams.tyNext < rayParams.tzNext) {
			auto hitPrimitive =
				EvaluatePrimitiveCollectionCell(currentCell, ray, tMin, tMax,
												intersectionResult, rayParams.tyNext);
			if (hitPrimitive != nullptr) {
				closestPrimitive = hitPrimitive;
				break;
			}
			
			rayParams.tyNext += rayParams.dty;
			rayParams.iy += rayParams.iyStep;
			if (rayParams.iy == rayParams.iyStop) {
				break;
			}
		}
		else {
			auto hitPrimitive =
				EvaluatePrimitiveCollectionCell(currentCell, ray, tMin, tMax,
												intersectionResult, rayParams.tzNext);
			if (hitPrimitive != nullptr) {
				closestPrimitive = hitPrimitive;
				break;
			}
			
			rayParams.tzNext += rayParams.dtz;
			rayParams.iz += rayParams.izStep;
			if (rayParams.iz == rayParams.izStop) {
				break;
			}
		}
	}
		
	return closestPrimitive;
}

Primitive* GridPrimitive::IntersectShadow(const Ray &ray, float tMin, float tMax) {
	for (auto currPrimitive : primitivesNotInCells) {
		auto hitPrim = currPrimitive->IntersectShadow(ray, tMin, tMax);
		if (hitPrim != nullptr) {
			return hitPrim;
		}
	}
	
#ifdef BRUTE_FORCE_TEST
	return BruteForceShadowFeelerIntersectsAnObject(ray, tMin, tMax);
#endif
	
	// the following code includes modifications
	// from Shirley and Morley (2003)
	// ported to Raytracing from the Ground Up
	RayParameters rayParams;
	if (!CheckBoundsOfRay(ray, tMin, tMax, rayParams)) {
		return nullptr;
	}
	
	while(true) {
		PrimitiveCollection& currentCell = cells[rayParams.ix +
			nx * rayParams.iy + nx * ny * rayParams.iz];
		if (!rayParams.txInvalid && rayParams.txNext < rayParams.tyNext
			&& rayParams.txNext < rayParams.tzNext) {
			auto hitPrimitive =
				IntersectAgainstPrimitiveCollectionShadow(currentCell, ray, tMin,
														  rayParams.txNext);
			
			if (hitPrimitive != nullptr) {
				return hitPrimitive;
			}
			
			rayParams.txNext += rayParams.dtx;
			rayParams.ix += rayParams.ixStep;
			if (rayParams.ix == rayParams.ixStop) {
				break;
			}
		}
		else if (!rayParams.tyInvalid && rayParams.tyNext < rayParams.tzNext) {
			auto hitPrimitive =
				IntersectAgainstPrimitiveCollectionShadow(currentCell, ray, tMin,
														  rayParams.tyNext);
			if (hitPrimitive != nullptr) {
				return hitPrimitive;
			}
			
			rayParams.tyNext += rayParams.dty;
			rayParams.iy += rayParams.iyStep;
			if (rayParams.iy == rayParams.iyStop) {
				break;
			}
		}
		else {
			auto hitPrimitive =
				IntersectAgainstPrimitiveCollectionShadow(currentCell, ray, tMin,
														  rayParams.tzNext);
			if (hitPrimitive != nullptr) {
				return hitPrimitive;
			}
			
			rayParams.tzNext += rayParams.dtz;
			rayParams.iz += rayParams.izStep;
			if (rayParams.iz == rayParams.izStop) {
				break;
			}
		}
	}
	
	return nullptr;
}

AABBox GridPrimitive::GetBoundingBox() const {
	return boundingBox;
}

Point3 GridPrimitive::ComputeMinCoordinates(std::vector<std::shared_ptr<Primitive>> const & primitives) {
	AABBox objectBBox;
	Point3 minCoord;
	bool xSet = false, ySet = false, zSet = false;
	size_t numPrimitives = primitives.size();
	
	for (size_t index = 0; index < numPrimitives; index++) {
		auto currPrimitive = primitives[index];
		// skip primitives that don't have a bounding box
		if (!currPrimitive->HasBoundingBox()) {
			continue;
		}
		
		objectBBox = currPrimitive->GetBoundingBox();
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


Point3 GridPrimitive::ComputeMaxCoordinates(std::vector<std::shared_ptr<Primitive>> const & primitives) {
	AABBox objectBBox;
	Point3 maxCoord;
	bool xSet = false, ySet = false, zSet = false;
	size_t numPrimitives = primitives.size();
	
	for (size_t index = 0; index < numPrimitives; index++) {
		auto currPrimitive = primitives[index];
		// skip primitives that don't have a bounding box
		if (!currPrimitive->HasBoundingBox()) {
			continue;
		}
		
		objectBBox = currPrimitive->GetBoundingBox();
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

bool GridPrimitive::CheckBoundsOfRay(Ray const &ray, float tMin, float tMax,
									 RayParameters& rayParams) {
	Point3 const & rayOrigin = ray.GetOrigin();
	Vector3 const & rayDirection = ray.GetDirection();
	float ox = rayOrigin[0];
	float oy = rayOrigin[1];
	float oz = rayOrigin[2];
	float dx = rayDirection[0];
	float dy = rayDirection[1];
	float dz = rayDirection[2];
	
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
	float a = 1.0f / dx;
	if (a >= 0) {
		txMin = (x0 - ox) * a;
		txMax = (x1 - ox) * a;
	}
	else {
		txMin = (x1 - ox) * a;
		txMax = (x0 - ox) * a;
	}
	
	float b = 1.0 / dy;
	if (b >= 0) {
		tyMin = (y0 - oy) * b;
		tyMax = (y1 - oy) * b;
	}
	else {
		tyMin = (y1 - oy) * b;
		tyMax = (y0 - oy) * b;
	}
	
	float c = 1.0 / dz;
	if (c >= 0) {
		tzMin = (z0 - oz) * c;
		tzMax = (z1 - oz) * c;
	}
	else {
		tzMin = (z1 - oz) * c;
		tzMax = (z0 - oz) * c;
	}
	
	// check bounds
	if (txMin > tMax || tyMin > tMax ||
		tzMin > tMax) {
		return false;
	}
	if (txMax < tMin || tyMax < tMin ||
		tzMax < tMin) {
		return false;
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
	
	// find cell coordinates of initial point
	if (boundingBox.PointInside(rayOrigin)) {
		rayParams.ix = (int)CommonMath::Clamp((ox - x0)*nx/(x1 - x0), 0, nx - 1);
		rayParams.iy = (int)CommonMath::Clamp((oy - y0)*ny/(y1 - y0), 0, ny - 1);
		rayParams.iz = (int)CommonMath::Clamp((oz - z0)*nz/(z1 - z0), 0, nz - 1);
	}
	else {
		// hit from the outside
		Point3 pointHit = ray.GetPositionAtParam(t0);
		rayParams.ix = (int)CommonMath::Clamp((pointHit[0] - x0)*nx/(x1 - x0), 0, nx - 1);
		rayParams.iy = (int)CommonMath::Clamp((pointHit[1] - y0)*ny/(y1 - y0), 0, ny - 1);
		rayParams.iz = (int)CommonMath::Clamp((pointHit[2] - z0)*nz/(z1 - z0), 0, nz - 1);
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
	if (dx > 0) {
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
	if (fabs(dx) < EPSILON) {
		rayParams.txInvalid = true;
		rayParams.ixStep = -1;
		rayParams.ixStop = -1;
	}
	
	if (dy > 0) {
		rayParams.tyNext = tyMin + (rayParams.iy + 1) * rayParams.dty;
		rayParams.iyStep = +1;
		rayParams.iyStop = ny;
	}
	else {
		rayParams.tyNext = tyMin + (ny - rayParams.iy) * rayParams.dty;
		rayParams.iyStep = -1;
		rayParams.iyStop = -1;
	}
	
	if (fabs(dy) < EPSILON) {
		rayParams.tyInvalid = true;
		rayParams.iyStep = -1;
		rayParams.iyStop = -1;
	}
	
	if (dz > 0) {
		rayParams.tzNext = tzMin + (rayParams.iz + 1) * rayParams.dtz;
		rayParams.izStep = +1;
		rayParams.izStop = nz;
	}
	else {
		rayParams.tzNext = tzMin + (nz - rayParams.iz) * rayParams.dtz;
		rayParams.izStep = -1;
		rayParams.izStop = -1;
	}
	
	if (fabs(dz) < EPSILON) {
		rayParams.tzInvalid = true;
		rayParams.izStep = -1;
		rayParams.izStop = -1;
	}
	
	return true;
}

Primitive* GridPrimitive::EvaluatePrimitiveCollectionCell(PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float& tMax, IntersectionResult &intersectionResult, float tNext) {
	float tMaxTest = tMax;
	IntersectionResult intersecResTemp;
	auto hitPrimitive = IntersectAgainstPrimitiveCollection(primitiveCollection,
															ray, tMin, tMaxTest,
															intersecResTemp);
	if (hitPrimitive != nullptr && tMaxTest < tNext) {
		tMax = tMaxTest;
		intersectionResult = intersecResTemp;
		intersectionResult.compoundPrimitiveToIntersectedPrim[this] = hitPrimitive;
		return hitPrimitive;
	}

	return nullptr;
}

Primitive* GridPrimitive::IntersectAgainstPrimitiveCollection(PrimitiveCollection &
	primitiveCollection, const Ray &ray, float tMin, float& tMax,
	IntersectionResult &intersectionResult) {
	auto & primitivesInCollection = primitiveCollection.primitives;
	unsigned int numElements = primitivesInCollection.size();
	
	Primitive * closestPrimSoFar = nullptr;
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitivesInCollection[index];
		IntersectionResult currResult;
		auto hitTest = currPrimitive->Intersect(ray, tMin, tMax, currResult);
		if (hitTest != nullptr) {
			closestPrimSoFar = hitTest;
			intersectionResult = currResult;
		}
	}
	
	return closestPrimSoFar;
}

Primitive* GridPrimitive::IntersectAgainstPrimitiveCollectionShadow(
																	PrimitiveCollection & primitiveCollection,
																	const Ray &ray, float tMin,
																	float tMax) {
	auto & primitivesInCollection = primitiveCollection.primitives;
	unsigned int numElements = primitivesInCollection.size();
	
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitivesInCollection[index];
		auto shadowPrimHit = currPrimitive->IntersectShadow(ray, tMin, tMax);
		if (shadowPrimHit != nullptr) {
			return shadowPrimHit;
		}
	}
	
	return nullptr;
}

Primitive* GridPrimitive::BruteForceIntersect(const Ray &ray, float tMin, float& tMax,
											  IntersectionResult &intersectionResult) {
	Primitive* primitiveHit = nullptr;
	for(auto primitiveCollection : cells) {
		IntersectionResult currResult;
		auto currPrimitiveHit =
			IntersectAgainstPrimitiveCollection(primitiveCollection, ray, tMin, tMax,
												currResult);
		if (currPrimitiveHit != nullptr) {
			primitiveHit = currPrimitiveHit;
			intersectionResult = currResult;
			intersectionResult.compoundPrimitiveToIntersectedPrim[this] = primitiveHit;
		}
	}
	
	return primitiveHit;
}

Primitive* GridPrimitive::BruteForceShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
																   float tMax) {
	for(auto primitiveCollection : cells) {
		auto primitiveHit =
			IntersectAgainstPrimitiveCollectionShadow(primitiveCollection, ray, tMin, tMax);
		if (primitiveHit != nullptr) {
			return primitiveHit;
		}
	}
	
	return nullptr;
}

Vector3 GridPrimitive::GetNormal(const ShadingInfo& shadingInfo) const {
	Primitive* childPrim = (*shadingInfo.compoundPrimitiveToIntersectedPrim)[(Primitive*)this];
	return childPrim != nullptr ? childPrim->GetNormal(shadingInfo) : Vector3::Zero();
}

Material const * GridPrimitive::GetMaterial(const ShadingInfo& shadingInfo) {
	Primitive* childPrim = (*shadingInfo.compoundPrimitiveToIntersectedPrim)[(Primitive*)this];
	return childPrim != nullptr ? childPrim->GetMaterial(shadingInfo) : material.get();
}

// not valid for this primitive
Vector3 GridPrimitive::ComputeHardNormal(Point3 const &position) const {
	return Vector3::Zero();
}

void GridPrimitive::SamplePrimitive(Point3& resultingSample,
									const ShadingInfo& shadingInfo) {
	Primitive* childPrim = (*shadingInfo.compoundPrimitiveToIntersectedPrim)[(Primitive*)this];
	if (childPrim != nullptr) {
		childPrim->SamplePrimitive(resultingSample, shadingInfo);
	}
}

const GenericSampler* GridPrimitive::GetSampler(const ShadingInfo& shadingInfo) {
	Primitive* childPrim = (*shadingInfo.compoundPrimitiveToIntersectedPrim)[(Primitive*)this];
	return childPrim != nullptr ? childPrim->GetSampler(shadingInfo) : nullptr;
}

float GridPrimitive::PDF(const ShadingInfo& shadingInfo) const {
	Primitive* childPrim = (*shadingInfo.compoundPrimitiveToIntersectedPrim)[(Primitive*)this];
	return childPrim != nullptr ? childPrim->PDF(shadingInfo) : 0.0f;
}

std::shared_ptr<Primitive> GridPrimitive::GetSubPrimitiveByName(std::string const & intersecPrimName) const {
	for(auto primitiveCollection : cells) {
		for (auto primitive : primitiveCollection.primitives) {
			if (primitive->GetName() == intersecPrimName) {
				return primitive;
			}
		}
	}
	
	return nullptr;
}
