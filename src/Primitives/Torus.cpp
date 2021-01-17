#include "Torus.h"
#include "CommonMath.h"
#include <algorithm>
#include <limits>

// use doubles for accuracy
Primitive* Torus::Intersect(const Ray &ray, float tMin, float& tMax,
							IntersectionResult &intersectionResult) {
	if (!boundingBox.RayHit(ray)) {
		return nullptr;
	}
	
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	
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
		return nullptr;
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
		return nullptr;
	}
		
	tMax = t;
	intersectionResult.rayIntersectT = tMax;
	
	return this;
}

Primitive* Torus::IntersectShadow(const Ray &ray, float tMin, float tMax) {
	if (!boundingBox.RayHit(ray)) {
		return nullptr;
	}
	
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	
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
		return nullptr;
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
		return nullptr;
	}
	
	return this;
}

Vector3 Torus::GetNormal(const ShadingInfo &shadingInfo) const {
	return ComputeHardNormal(shadingInfo.intersectionPosition);
}

Vector3 Torus::ComputeHardNormal(Point3 const &position) const {
	Vector3 normal;
	float paramSquared = sweptRadiusSquared + tubeRadiusSquared;
	float x = position[0];
	float y = position[1];
	float z = position[2];
	float sumSquared = x * x + y * y + z * z;
	
	normal[0] = 4.0f * x * (sumSquared - paramSquared);
	normal[1] = 4.0f * y * (sumSquared - paramSquared + 2.0 * sweptRadiusSquared);
	normal[2] = 4.0f * z * (sumSquared - paramSquared);
	normal.Normalize();
	
	return normal;
}

void Torus::SamplePrimitive(Point3& resultingSample,
							const ShadingInfo &shadingInfo) {
	// nothing to see here for now
}

// each sample's probability is 1.0/inverseArea
float Torus::PDF(const ShadingInfo &shadingInfo) const {
	return 1.0f;
}

AABBox Torus::GetBoundingBox() const {
	return boundingBox;
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
