
#include "GridPrimitive.h"
#include "Math/CommonMath.h"

bool GridPrimitive::Intersect(const Ray &ray, float tMin, float& tMax,
							  IntersectionResult &intersectionResult) {
	Primitive* closestPrimitive = nullptr;
	bool hitBefore = false;
	for (auto currPrimitive : primitivesNotInCells) {
		if (currPrimitive->Intersect(ray, tMin, tMax,
									 intersectionResult)) {
			closestPrimitive = currPrimitive;
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

bool GridPrimitive::IntersectShadow(const Ray &ray, float tMin, float tMax) {
	/*for (auto currPrimitive : primitivesNotInCells) {
		if (currPrimitive == primitiveToExclude) {
			continue;
		}
		
		if (currPrimitive->IntersectShadow(ray, tMin, tMax)) {
			return true;
		}
	}
	
#ifdef BRUTE_FORCE_TEST
	return BruteForceShadowFeelerIntersectsAnObject(ray, tMin, tMax, primitiveToExclude);
#endif
	
	// the following code includes modifications
	// from Shirley and Morley (2003)
	// ported to Raytracing from the Ground Up
	RayParameters rayParams;
	if (!CheckBoundsOfRay(ray, tMin, tMax, rayParams)) {
		return false;
	}
	
	while(true) {
		PrimitiveCollection& currentCell = cells[rayParams.ix +
			nx * rayParams.iy + nx * ny * rayParams.iz];
		if (!rayParams.txInvalid && rayParams.txNext < rayParams.tyNext
			&& rayParams.txNext < rayParams.tzNext) {
			auto hitPrimitive =
				EvaluatePrimitiveCollectionCellShadow(currentCell, ray, tMin,
													  rayParams.txNext,
													  primitiveToExclude);
			
			if (hitPrimitive) {
				return true;
			}
			
			rayParams.txNext += rayParams.dtx;
			rayParams.ix += rayParams.ixStep;
			if (rayParams.ix == rayParams.ixStop) {
				break;
			}
		}
		else if (!rayParams.tyInvalid && rayParams.tyNext < rayParams.tzNext) {
			auto hitPrimitive =
				EvaluatePrimitiveCollectionCellShadow(currentCell, ray, tMin,
													  rayParams.tyNext, primitiveToExclude);
			if (hitPrimitive) {
				return true;
			}
			
			rayParams.tyNext += rayParams.dty;
			rayParams.iy += rayParams.iyStep;
			if (rayParams.iy == rayParams.iyStop) {
				break;
			}
		}
		else {
			auto hitPrimitive =
				EvaluatePrimitiveCollectionCellShadow(currentCell, ray, tMin,
													  rayParams.tzNext, primitiveToExclude);
			if (hitPrimitive) {
				return true;
			}
			
			rayParams.tzNext += rayParams.dtz;
			rayParams.iz += rayParams.izStep;
			if (rayParams.iz == rayParams.izStop) {
				break;
			}
		}
	}*/
	
	return false;
}

Vector3 GridPrimitive::GetNormal(ParamsForNormal const &paramsForNormal) const {
	// TODO
	return Vector3();
}

Vector3 GridPrimitive::ComputeHardNormal(Point3 const &position) const {
	// TODO
	return Vector3();
}

void GridPrimitive::SamplePrimitive(Point3& resultingSample,
									IntersectionResult const & intersectionResult) {
	// TODO sample last primitive found
}

float GridPrimitive::PDF(ParamsForNormal const &paramsForNormal) const {
	// TODO sample last primitive found
	return 1.0f;
}

AABBox GridPrimitive::GetBoundingBox() const {
	return boundingBox;
}

Point3 GridPrimitive::ComputeMinCoordinates(std::vector<Primitive*> const & primitives) {
	AABBox objectBBox;
	Point3 minCoord;
	bool xSet = false, ySet = false, zSet = false;
	size_t numPrimitives = primitives.size();
	
	for (size_t index = 0; index < numPrimitives; index++) {
		auto* currPrimitive = primitives[index];
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


Point3 GridPrimitive::ComputeMaxCoordinates(std::vector<Primitive*> const & primitives) {
	AABBox objectBBox;
	Point3 maxCoord;
	bool xSet = false, ySet = false, zSet = false;
	size_t numPrimitives = primitives.size();
	
	for (size_t index = 0; index < numPrimitives; index++) {
		auto* currPrimitive = primitives[index];
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
	
	// constrict max
	if (t1 > tMax) {
		t1 = tMax;
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

Primitive* GridPrimitive::IntersectAgainstPrimitiveCollection(PrimitiveCollection &
	primitiveCollection, const Ray &ray, float tMin, float& tMax,
	IntersectionResult &intersectionResult) {
	auto & primitivesInCollection = primitiveCollection.primitives;
	unsigned int numElements = primitivesInCollection.size();
	
	Primitive * closestPrimSoFar = nullptr;
	IntersectionResult tempRes;
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitivesInCollection[index];

		// if we test against multiple compound objects in a row, reset primitive
		// intersection data from previous tests that might have returned true
		tempRes.ResetPrimIntersectionData();

		if (currPrimitive->Intersect(ray, tMin, tMax, tempRes)) {
			closestPrimSoFar = currPrimitive;
			// TODO: try to avoid copy somehow, this is gross
			intersectionResult = tempRes;
		}
	}
	
	return closestPrimSoFar;
}

bool GridPrimitive::IntersectAgainstPrimitiveCollectionShadow(
	PrimitiveCollection & primitiveCollection, const Ray &ray, float tMin, float tMax,
	const Primitive* primitiveToExclude) {
	auto & primitivesInCollection = primitiveCollection.primitives;
	unsigned int numElements = primitivesInCollection.size();
	
	for (unsigned int index = 0; index < numElements; index++) {
		auto currPrimitive = primitivesInCollection[index];
		
		if (currPrimitive == primitiveToExclude) {
			continue;
		}

		if (currPrimitive->IntersectShadow(ray, tMin, tMax)) {
			return true;
		}
	}
	
	return false;
}

Primitive* GridPrimitive::BruteForceIntersect(const Ray &ray, float tMin, float& tMax,
											  IntersectionResult &intersectionResult) {
	Primitive* primitiveHit = nullptr;
	for(auto primitiveCollection : cells) {
		auto currPrimitiveHit =
		IntersectAgainstPrimitiveCollection(primitiveCollection, ray, tMin, tMax, intersectionResult);
		if (currPrimitiveHit != nullptr) {
			primitiveHit = currPrimitiveHit;
		}
	}
	
	return primitiveHit;
}

bool GridPrimitive::BruteForceShadowFeelerIntersectsAnObject(const Ray& ray, float tMin,
															   float tMax,
															   const Primitive* primitiveToExclude) {
	for(auto primitiveCollection : cells) {
		auto didHitPrimitive =
		IntersectAgainstPrimitiveCollectionShadow(primitiveCollection, ray, tMin, tMax, primitiveToExclude);
		if (didHitPrimitive) {
			return true;
		}
	}
	
	return false;
}
