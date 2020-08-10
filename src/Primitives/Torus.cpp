#include "Torus.h"
#include "CommonMath.h"
#include <algorithm>
#include <limits>

// use doubles for accuracy
bool Torus::IntersectLocal(const Ray &rayLocal, float tMin, float& tMax,
					IntersectionResult &intersectionResult) {
	if (!boundingBox.RayHit(rayLocal)) {
		return false;
	}
	
	const Point3& rayOrigin = rayLocal.GetOrigin();
	const Vector3& rayDirection = rayLocal.GetDirection();
	
	double rayOriginX = rayOrigin[0];
	double rayOriginY = rayOrigin[1];
	double rayOriginZ = rayOrigin[2];
	double rayDirX = rayDirection[0];
	double rayDirY = rayDirection[1];
	double rayDirZ = rayDirection[2];
	
	// coefficient array for the quartic equation
	double coeffs[5];
	// solution array for the quartic equation
	double roots[4];
	
	// define the coefficients of the quartic equation
	double sumDirSqrd 	= rayDirX * rayDirX + rayDirY * rayDirY + rayDirZ * rayDirZ;
	double e				= rayOriginX * rayOriginX + rayOriginY * rayOriginY +
						rayOriginZ * rayOriginZ - sweptRadiusSquared - tubeRadiusSquared;
	double f			= rayOriginX * rayDirX + rayOriginY * rayDirY + rayOriginZ * rayDirZ;
	double fourASqrd	= 4.0 * sweptRadiusSquared;
	
	// coeffs[0] is constant term
	coeffs[0] = e * e - fourASqrd * (tubeRadiusSquared - rayOriginY * rayOriginY);
	coeffs[1] = 4.0 * f * e + 2.0 * fourASqrd * rayOriginY * rayDirY;
	coeffs[2] = 2.0 * sumDirSqrd * e + 4.0 * f * f + fourASqrd * rayDirY * rayDirY;
	coeffs[3] = 4.0 * sumDirSqrd * f;
	// coeffs[4] is coefficient of t^4
	coeffs[4] = sumDirSqrd * sumDirSqrd;
	
	// find roots of the quartic equation
	int numRealRoots 	= CommonMath::SolveQuartic(coeffs, roots);
	bool	intersected = false;
	float 	t 		 	= std::numeric_limits<float>::max();
	
	// ray misses the torus
	if (numRealRoots == 0) {
		return false;
	}
	
	// find the smallest root greater than kEpsilon, if any
	// the roots array is not sorted
	for (int j = 0; j < numRealRoots; j++) {
		if (roots[j] < tMax && roots[j] > tMin) {
			intersected = true;
			t = roots[j];
		}
	}
		
	if(!intersected) {
		return false;
	}
		
	tMax = t;
	intersectionResult.SetIntersectionT(tMax);
	
	return true;
}

bool Torus::IntersectShadowLocal(const Ray &rayLocal, float tMin, float tMax) {
	if (!boundingBox.RayHit(rayLocal)) {
		return false;
	}
	
	const Point3& rayOrigin = rayLocal.GetOrigin();
	const Vector3& rayDirection = rayLocal.GetDirection();
	
	double rayOriginX = rayOrigin[0];
	double rayOriginY = rayOrigin[1];
	double rayOriginZ = rayOrigin[2];
	double rayDirX = rayDirection[0];
	double rayDirY = rayDirection[1];
	double rayDirZ = rayDirection[2];
	
	// coefficient array for the quartic equation
	double coeffs[5];
	// solution array for the quartic equation
	double roots[4];
	
	// define the coefficients of the quartic equation
	double sumDirSqrd 	= rayDirX * rayDirX + rayDirY * rayDirY + rayDirZ * rayDirZ;
	double e			= rayOriginX * rayOriginX + rayOriginY * rayOriginY +
		rayOriginZ * rayOriginZ - sweptRadiusSquared - tubeRadiusSquared;
	double f			= rayOriginX * rayDirX + rayOriginY * rayDirY + rayOriginZ * rayDirZ;
	double fourASqrd	= 4.0 * sweptRadiusSquared;
	
	// coeffs[0] is constant term
	coeffs[0] = e * e - fourASqrd * (tubeRadiusSquared - rayOriginY * rayOriginY);
	coeffs[1] = 4.0 * f * e + 2.0 * fourASqrd * rayOriginY * rayDirY;
	coeffs[2] = 2.0 * sumDirSqrd * e + 4.0 * f * f + fourASqrd * rayDirY * rayDirY;
	coeffs[3] = 4.0 * sumDirSqrd * f;
	// coeffs[4] is coefficient of t^4
	coeffs[4] = sumDirSqrd * sumDirSqrd;
	
	// find roots of the quartic equation
	int numRealRoots = CommonMath::SolveQuartic(coeffs, roots);
	bool	intersected = false;
	float 	t 		 	= std::numeric_limits<float>::max();
	// ray misses the torus
	if (numRealRoots == 0) {
		return false;
	}
	
	// find the smallest root greater than kEpsilon, if any
	// the roots array is not sorted
	for (int j = 0; j < numRealRoots; j++) {
		if (roots[j] < tMax && roots[j] > tMin) {
			t = roots[j];
			intersected = true;
		}
	}
		
	if (!intersected) {
		return false;
	}
	
	return true;
}

Vector3 Torus::GetNormalWorld(IntersectionResult const &intersectionResult) const {
	Vector3 normal;
	float paramSquared = sweptRadiusSquared + tubeRadiusSquared;

	auto intersecPos = intersectionResult.GetIntersectionPosLocal();
	float x = intersecPos[0];
	float y = intersecPos[1];
	float z = intersecPos[2];
	float sumSquared = x * x + y * y + z * z;
	
	normal[0] = 4.0f * x * (sumSquared - paramSquared);
	normal[1] = 4.0f * y * (sumSquared - paramSquared + 2.0 * sweptRadiusSquared);
	normal[2] = 4.0f * z * (sumSquared - paramSquared);
	normal.Normalize();
	
	return isTransformed ? GetWorldToLocalTransposeDir(normal) : normal;
}

void Torus::SamplePrimitiveLocal(Point3& resultingSample) {
	// nothing to see here for now
}

void Torus::SamplePrimitiveWorld(Point3& resultingSample) {
	// nothing to see here for now
}

// each sample's probability is 1.0/inverseArea
float Torus::PDF(const IntersectionResult& intersectionResult) const {
	return 1.0f;
}

AABBox Torus::GetBoundingBoxLocal() const {
	return boundingBox;
}

AABBox Torus::GetBoundingBoxWorld() const {
	auto minPoint = boundingBox.GetMin();
	auto maxPoint = boundingBox.GetMax();
	return AABBox(GetLocalToWorldPos(minPoint),
				  GetLocalToWorldPos(maxPoint));
}

void Torus::Initialize() {
	sweptRadiusSquared = sweptRadius*sweptRadius;
	tubeRadiusSquared = tubeRadius*tubeRadius;
	boundingBox = AABBox(-sweptRadius - tubeRadius,
						 -tubeRadius,
						 -sweptRadius - tubeRadius,
						 sweptRadius + tubeRadius,
						 tubeRadius,
						 sweptRadius + tubeRadius);
}
