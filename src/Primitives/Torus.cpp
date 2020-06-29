#include "Torus.h"
#include "CommonMath.h"
#include <algorithm>
#include <limits>

bool Torus::Intersect(const Ray &ray, float tMin, float& tMax,
					IntersectionResult &intersectionResult) {
	if (!boundingBox.RayHit(ray)) {
		return false;
	}
	
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	
	float rayOriginX = rayOrigin[0];
	float rayOriginY = rayOrigin[1];
	float rayOriginZ = rayOrigin[2];
	float rayDirX = rayDirection[0];
	float rayDirY = rayDirection[1];
	float rayDirZ = rayDirection[2];
	
	// coefficient array for the quartic equation
	float coeffs[5];
	// solution array for the quartic equation
	float roots[4];
	
	// define the coefficients of the quartic equation
	float sumDirSqrd 	= rayDirX * rayDirX + rayDirY * rayDirY + rayDirZ * rayDirZ;
	float e				= rayOriginX * rayOriginX + rayOriginY * rayOriginY +
						rayOriginZ * rayOriginZ - sweptRadiusSquared - tubeRadiusSquared;
	float f			= rayOriginX * rayDirX + rayOriginY * rayDirY + rayOriginZ * rayDirZ;
	float fourASqrd	= 4.0 * sweptRadiusSquared;
	
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

bool Torus::IntersectShadow(const Ray &ray, float tMin, float tMax)
{
	if (!boundingBox.RayHit(ray)) {
		return false;
	}
	
	const Point3& rayOrigin = ray.GetOrigin();
	const Vector3& rayDirection = ray.GetDirection();
	
	float rayOriginX = rayOrigin[0];
	float rayOriginY = rayOrigin[1];
	float rayOriginZ = rayOrigin[2];
	float rayDirX = rayDirection[0];
	float rayDirY = rayDirection[1];
	float rayDirZ = rayDirection[2];
	
	// coefficient array for the quartic equation
	float coeffs[5];
	// solution array for the quartic equation
	float roots[4];
	
	// define the coefficients of the quartic equation
	float sumDirSqrd 	= rayDirX * rayDirX + rayDirY * rayDirY + rayDirZ * rayDirZ;
	float e			= rayOriginX * rayOriginX + rayOriginY * rayOriginY +
		rayOriginZ * rayOriginZ - sweptRadiusSquared - tubeRadiusSquared;
	float f			= rayOriginX * rayDirX + rayOriginY * rayDirY + rayOriginZ * rayDirZ;
	float fourASqrd	= 4.0 * sweptRadiusSquared;
	
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

void Torus::SamplePrimitive(Point3& resultingSample) {
	// nothing to see here for now
}

// each sample's probability is 1.0/inverseArea
float Torus::PDF(const IntersectionResult& intersectionResult) const {
	return 1.0f;
}

AABBox Torus::GetBoundingBox() const {
	return boundingBox;
}

